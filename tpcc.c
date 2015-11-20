#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "partition.h"
#include "smphashtable.h"
#include "util.h"
#include "tpcc.h"

uint64_t set_last_name(uint64_t num, char* name)
{
  static const char *n[] =
  {"BAR", "OUGHT", "ABLE", "PRI", "PRES",
    "ESE", "ANTI", "CALLY", "ATION", "EING"};

  strcpy(name, n[num/100]); 
  strcat(name, n[(num/10)%10]);
  strcat(name, n[num%10]);
  return strlen(name);
}

uint64_t cust_derive_key(char * c_last, uint64_t c_d_id, uint64_t c_w_id)
{
  uint64_t key = 0;
  char offset = 'A';
  for (uint32_t i = 0; i < strlen(c_last); i++)
    key = (key << 1) + (c_last[i] - offset);
  key = key << 10;
  key += c_w_id * TPCC_NDIST_PER_WH + c_d_id;
  return key;
}

static void load_stock(int w_id, struct partition *p)
{
  struct elem *e;
  hash_key key, base_sid, sid;
  struct tpcc_stock *r;
    
  base_sid = w_id * TPCC_MAX_ITEMS;
  for (int i = 1; i <= TPCC_MAX_ITEMS; i++) {
    sid = base_sid + i;
    key = MAKE_HASH_KEY(STOCK_TID, sid);
    e = hash_insert(p, key, sizeof(struct tpcc_stock), NULL);
    assert(e);

    r = (struct tpcc_stock *) e->value;
    r->s_i_id = i;
    r->s_w_id = w_id;
    r->s_quantity = URand(10, 100);
    for (int j = 0; j < 10; j++)
      make_alpha_string(24, 24, r->s_dist[j]);

    r->s_ytd = 0;
    r->s_order_cnt = 0;
    r->s_remote_cnt = 0;
    
    int len = make_alpha_string(26, 50, r->s_data);
    if (RAND(100) < 10) {
      int idx = URand(0, len - 8); 
      memcpy(&r->s_data[idx], "original", 8);
    }

    p->ninserts++;
    e->ref_count++;
  }
}

static void load_history(int w_id, struct partition *p)
{
  struct elem *e;
  struct tpcc_history *r;
  hash_key key, pkey;

  for (int d = 1; d <= TPCC_NDIST_PER_WH; d++) {
    for (int c = 1; c <= TPCC_NCUST_PER_DIST; c++) {
      pkey = MAKE_CUST_KEY(w_id, d, c);
      key = MAKE_HASH_KEY(HISTORY_TID, pkey);

      e = hash_insert(p, key, sizeof(struct tpcc_history), NULL);
      assert(e);

      r = (struct tpcc_history *) e->value;
      r->h_c_id = c;
      r->h_c_d_id = d;
      r->h_c_w_id = w_id;
      r->h_d_id = d;
      r->h_w_id = w_id;
      r->h_date = 0;
      r->h_amount = 10.0;
      make_alpha_string(12, 24, r->h_data);

      p->ninserts++;
      e->ref_count++;
    }
  }
}

static void load_customer(int w_id, struct partition *p)
{
  struct elem *e;
  struct tpcc_customer *r;
  hash_key key;
  int last_name_len;

  for (int d = 1; d <= TPCC_NDIST_PER_WH; d++) {
    for (int c = 1; c <= TPCC_NCUST_PER_DIST; c++) {
      uint64_t ckey = MAKE_CUST_KEY(w_id, d, c);
      key = MAKE_HASH_KEY(CUSTOMER_TID, ckey);

      e = hash_insert(p, key, sizeof(struct tpcc_customer), NULL);
      assert(e);

      r = (struct tpcc_customer *) e->value;
      r->c_id = c;
      r->c_d_id = d;
      r->c_w_id = w_id;

      if (c <= 1000)
        last_name_len = set_last_name(c - 1, r->c_last);
      else
        last_name_len = set_last_name(NURand(255,0,999), r->c_last);

      memcpy(r->c_middle, "OE", 2);

      make_alpha_string(FIRST_NAME_MIN_LEN, FIRST_NAME_LEN, r->c_first);

      make_alpha_string(10, 20, r->c_street[0]);
      make_alpha_string(10, 20, r->c_street[1]);
      make_alpha_string(10, 20, r->c_city);
      make_alpha_string(2, 2, r->c_state); /* State */
      make_numeric_string(9, 9, r->c_zip); /* Zip */
      make_numeric_string(16, 16, r->c_phone); /* Zip */
      r->c_since = 0;
      r->c_credit_lim = 50000;
      r->c_delivery_cnt = 0;
      make_alpha_string(300, 500, r->c_data);

      if (RAND(10) == 0) {
        r->c_credit[0] = 'G';
      } else {
        r->c_credit[0] = 'B';
      }
      r->c_credit[1] = 'C';
      r->c_discount = (double)RAND(5000) / 10000;
      r->c_balance = -10.0;
      r->c_ytd_payment = 10.0;
      r->c_payment_cnt = 1;

      /* XXX: create secondary index using the main hash table itself. 
       * we can do this by deriving a key from the last name,dist,wh id 
       * and using it to create a new record which will contain both
       * the <last name,did,wid> secondary key and real key of the record
       */
#if 0
      hash_key dkey;
      dkey = cust_derive_key(r->c_last, d, w);

      // now we insert another record for secondary index
      struct elem *sie = hash_insert(p, dkey, sizeof(struct secondary_record), NULL);
      assert(sie);
    
      struct secondary_record *sr = (struct secondary_record *)sie->value;
      strcpy(sr->sr_last_name, r->c_last);
      sr->sr_rid = r->ckey;
      sie->ref_count++;
#endif

      p->ninserts++;
      e->ref_count++;
    }
  }
}

void init_permutation(uint64_t *cperm)
{
    int i;

    for(i = 0; i < TPCC_NCUST_PER_DIST; i++) {
        cperm[i] = i+1;
    }

    // shuffle
    for(i = 0; i < TPCC_NCUST_PER_DIST - 1; i++) {
        uint64_t j = URand(i + 1, TPCC_NCUST_PER_DIST - 1);
        uint64_t tmp = cperm[i];
        cperm[i] = cperm[j];
        cperm[j] = tmp;
    }

    return;
}


static void load_order(int w_id, struct partition *p)
{
  struct elem *e;
  struct tpcc_order *r;
  hash_key key, ckey;

  uint64_t *cperm = malloc(sizeof(uint64_t) * TPCC_NCUST_PER_DIST);
  assert(cperm);

  for (int d = 1; d <= TPCC_NDIST_PER_WH; d++) {
    init_permutation(cperm); 

    for (int o = 1; o <= TPCC_NCUST_PER_DIST; o++) {
      ckey = MAKE_CUST_KEY(w_id, d, o);
      key = MAKE_HASH_KEY(ORDER_TID, ckey);

      e = hash_insert(p, key, sizeof(struct tpcc_order), NULL);
      assert(e);

      p->ninserts++;
      e->ref_count++;

      r = (struct tpcc_order *) e->value;

      uint64_t c_id = cperm[o - 1];
      r->o_id = o;
      r->o_c_id = c_id;
      r->o_d_id = d;
      r->o_w_id = w_id;
      uint64_t o_entry = 2013;
      r->o_entry_d = 2013;
      if (o < 2101)
        r->o_carrier_id = URand(1, 10);
      else
        r->o_carrier_id = 0;
      uint64_t o_ol_cnt = URand(5, 15);
      r->o_ol_cnt = o_ol_cnt;
      r->o_all_local = 1;

      for (int ol = 1; ol <= o_ol_cnt; ol++) {
        hash_key ol_pkey = MAKE_OL_KEY(w_id, d, o, ol);
        hash_key ol_key = MAKE_HASH_KEY(ORDER_LINE_TID, ol_pkey);

        struct elem *e_ol = hash_insert(p, ol_key, 
            sizeof(struct tpcc_order_line), NULL);
        assert(e_ol);

        p->ninserts++;
        e_ol->ref_count++;

        struct tpcc_order_line *r_ol = (struct tpcc_order_line *) e->value;
        r_ol->ol_o_id = o;
        r_ol->ol_d_id = d;
        r_ol->ol_w_id = w_id;
        r_ol->ol_number = ol;
        r_ol->ol_i_id = URand(1, 100000);
        r_ol->ol_supply_w_id = w_id;

        if (o < 2101) {
          r_ol->ol_delivery_d = o_entry;
          r_ol->ol_amount = 0;
        } else {
          r_ol->ol_delivery_d = 0;
          r_ol->ol_amount = (double)URand(1, 999999)/100;
        }
        r_ol->ol_quantity = 5;
        make_alpha_string(24, 24, r_ol->ol_dist_info);

        //          uint64_t key = orderlineKey(wid, did, oid);
        //          index_insert(i_orderline, key, row, wh_to_part(wid));

        //          key = distKey(did, wid);
        //          index_insert(i_orderline_wd, key, row, wh_to_part(wid));
      }

      // NEW ORDER
      if (o > 2100) {
        key = MAKE_HASH_KEY(NEW_ORDER_TID, ckey);
        struct elem *e_no = hash_insert(p, key, 
            sizeof(struct tpcc_new_order), NULL);
        assert(e_no);

        p->ninserts++;
        e_no->ref_count++;

        struct tpcc_new_order *r_no = (struct tpcc_new_order *) e_no->value;
 
        r_no->no_o_id = o;
        r_no->no_d_id = d;
        r_no->no_w_id = w_id;
      }
    }
  }

  free(cperm);
}

void load_item(int w_id, struct partition *p)
{
  struct elem *e;
  struct tpcc_item *r;
  hash_key key;
  int data_len;

  if (w_id != 1) {
    printf("Client %d skipping loading item table\n", w_id);
  }
    
  for (int i = 1; i <= TPCC_MAX_ITEMS; i++) {
    key = MAKE_HASH_KEY(ITEM_TID, i);
    e = hash_insert(p, key, sizeof(struct tpcc_item), NULL);
    assert(e);

    p->ninserts++;
    e->ref_count++;

    r = (struct tpcc_item *) e->value;
    r->i_id = i;
    r->i_im_id = URand(1L,10000L);
    make_alpha_string(14, 24, r->i_name);
    r->i_price = URand(1, 100);
    data_len = make_alpha_string(26, 50, r->i_data);

    // TODO in TPCC, "original" should start at a random position
    if (RAND(10) == 0) {
      int idx = URand(0, data_len - 8);
      memcpy(&r->i_data[idx], "original", 8);
    }
  }
}

void load_warehouse(int w_id, struct partition *p)
{
  struct elem *e;
  struct tpcc_warehouse *r;
  hash_key key;

  key = MAKE_HASH_KEY(WAREHOUSE_TID, w_id);
  e = hash_insert(p, key, sizeof(struct tpcc_warehouse), NULL);
  assert(e);

  p->ninserts++;
  e->ref_count++;

  r = (struct tpcc_warehouse *) e->value;
  r->w_id = w_id;

  make_alpha_string(6, 10, r->w_name);
  make_alpha_string(10, 20, r->w_street[0]);
  make_alpha_string(10, 20, r->w_street[1]);
  make_alpha_string(10, 20, r->w_city);
  make_alpha_string(2, 2, r->w_state);
  make_alpha_string(9, 9, r->w_zip);
  double tax = (double)URand(0L,200L)/1000.0;
  double w_ytd=300000.00;
  r->w_tax = tax;
  r->w_ytd = w_ytd;
}

void load_district(int w_id, struct partition *p)
{
  struct elem *e;
  struct tpcc_district *r;
  hash_key key, dkey;

  for (int d = 1; d <= TPCC_NDIST_PER_WH; d++) {
    dkey = MAKE_DIST_KEY(w_id, d);
    key = MAKE_HASH_KEY(DISTRICT_TID, dkey);
    e = hash_insert(p, key, sizeof(struct tpcc_district), NULL);
    assert(e);

    p->ninserts++;
    e->ref_count++;

    r = (struct tpcc_district *) e->value;
    r->d_id = d;
    r->d_w_id = w_id;

    make_alpha_string(6, 10, r->d_name);
    make_alpha_string(10, 20, r->d_street[0]);
    make_alpha_string(10, 20, r->d_street[1]);
    make_alpha_string(10, 20, r->d_city);
    make_alpha_string(2, 2, r->d_state);
    make_alpha_string(9, 9, r->d_zip);
    double tax = (double)URand(0L,200L)/1000.0;
    double w_ytd=30000.00;
    r->d_tax = tax;
    r->d_ytd = w_ytd;
    r->d_next_o_id = 3001;
  }
}

void tpcc_load_data(int id, struct partition *p, struct partition *item)
{
  load_stock(id, p);
  load_history(id, p);
  load_customer(id, p);
  load_order(id, p);
  if (id == 1)
    load_item(id, item);

  load_warehouse(id, p);
  load_district(id, p);
}
