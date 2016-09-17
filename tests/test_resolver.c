/* test_resolver.c
 * strophe XMPP client library -- tests for resolver
 *
 * Copyright (C) 2015 Dmitry Podgorny <pasis.ua@gmail.com>
 *
 *  This software is provided AS-IS with no warranty, either express
 *  or implied.
 *
 *  This program is dual licensed under the MIT and GPLv3 licenses.
 */

#include <assert.h>
#include <stdio.h>

#include "mesode.h"
#include "rand.h"
#include "resolver.h"
#include "test.h"

/* res_query("_xmpp-client._tcp.jabber.kiev.ua", C_IN, T_SRV, ...) */
static const unsigned char data1[] = {
    0x95, 0xf3, 0x81, 0x80, 0x00, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x0c, 0x5f, 0x78, 0x6d,
    0x70, 0x70, 0x2d, 0x63, 0x6c, 0x69, 0x65, 0x6e,
    0x74, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x06, 0x6a,
    0x61, 0x62, 0x62, 0x65, 0x72, 0x04, 0x6b, 0x69,
    0x65, 0x76, 0x02, 0x75, 0x61, 0x00, 0x00, 0x21,
    0x00, 0x01, 0xc0, 0x0c, 0x00, 0x21, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x3b, 0x00, 0x16, 0x00, 0x01,
    0x00, 0x00, 0x14, 0x66, 0x06, 0x6a, 0x61, 0x62,
    0x62, 0x65, 0x72, 0x04, 0x6b, 0x69, 0x65, 0x76,
    0x02, 0x75, 0x61, 0x00,
};
/* res_query("_xmpp-client._tcp.jabber.org", C_IN, T_SRV, ...) */
static const unsigned char data2[] = {
    0xf2, 0x98, 0x81, 0x80, 0x00, 0x01, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x0c, 0x5f, 0x78, 0x6d,
    0x70, 0x70, 0x2d, 0x63, 0x6c, 0x69, 0x65, 0x6e,
    0x74, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x06, 0x6a,
    0x61, 0x62, 0x62, 0x65, 0x72, 0x03, 0x6f, 0x72,
    0x67, 0x00, 0x00, 0x21, 0x00, 0x01, 0xc0, 0x0c,
    0x00, 0x21, 0x00, 0x01, 0x00, 0x00, 0x03, 0x83,
    0x00, 0x1a, 0x00, 0x1e, 0x00, 0x1e, 0x14, 0x66,
    0x07, 0x68, 0x65, 0x72, 0x6d, 0x65, 0x73, 0x32,
    0x06, 0x6a, 0x61, 0x62, 0x62, 0x65, 0x72, 0x03,
    0x6f, 0x72, 0x67, 0x00, 0xc0, 0x0c, 0x00, 0x21,
    0x00, 0x01, 0x00, 0x00, 0x03, 0x83, 0x00, 0x1c,
    0x00, 0x1f, 0x00, 0x1e, 0x14, 0x66, 0x09, 0x68,
    0x65, 0x72, 0x6d, 0x65, 0x73, 0x32, 0x76, 0x36,
    0x06, 0x6a, 0x61, 0x62, 0x62, 0x65, 0x72, 0x03,
    0x6f, 0x72, 0x67, 0x00,
};
/* res_query("_xmpp-client._tcp.gmail.com", C_IN, T_SRV, ...) */
static const unsigned char data3[] = {
    0xda, 0xa8, 0x81, 0x80, 0x00, 0x01, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x0c, 0x5f, 0x78, 0x6d,
    0x70, 0x70, 0x2d, 0x63, 0x6c, 0x69, 0x65, 0x6e,
    0x74, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x67,
    0x6d, 0x61, 0x69, 0x6c, 0x03, 0x63, 0x6f, 0x6d,
    0x00, 0x00, 0x21, 0x00, 0x01, 0xc0, 0x0c, 0x00,
    0x21, 0x00, 0x01, 0x00, 0x00, 0x02, 0x43, 0x00,
    0x1e, 0x00, 0x14, 0x00, 0x00, 0x14, 0x66, 0x04,
    0x61, 0x6c, 0x74, 0x31, 0x04, 0x78, 0x6d, 0x70,
    0x70, 0x01, 0x6c, 0x06, 0x67, 0x6f, 0x6f, 0x67,
    0x6c, 0x65, 0x03, 0x63, 0x6f, 0x6d, 0x00, 0xc0,
    0x0c, 0x00, 0x21, 0x00, 0x01, 0x00, 0x00, 0x02,
    0x43, 0x00, 0x1e, 0x00, 0x14, 0x00, 0x00, 0x14,
    0x66, 0x04, 0x61, 0x6c, 0x74, 0x34, 0x04, 0x78,
    0x6d, 0x70, 0x70, 0x01, 0x6c, 0x06, 0x67, 0x6f,
    0x6f, 0x67, 0x6c, 0x65, 0x03, 0x63, 0x6f, 0x6d,
    0x00, 0xc0, 0x0c, 0x00, 0x21, 0x00, 0x01, 0x00,
    0x00, 0x02, 0x43, 0x00, 0x1e, 0x00, 0x14, 0x00,
    0x00, 0x14, 0x66, 0x04, 0x61, 0x6c, 0x74, 0x32,
    0x04, 0x78, 0x6d, 0x70, 0x70, 0x01, 0x6c, 0x06,
    0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x03, 0x63,
    0x6f, 0x6d, 0x00, 0xc0, 0x0c, 0x00, 0x21, 0x00,
    0x01, 0x00, 0x00, 0x02, 0x43, 0x00, 0x19, 0x00,
    0x05, 0x00, 0x00, 0x14, 0x66, 0x04, 0x78, 0x6d,
    0x70, 0x70, 0x01, 0x6c, 0x06, 0x67, 0x6f, 0x6f,
    0x67, 0x6c, 0x65, 0x03, 0x63, 0x6f, 0x6d, 0x00,
    0xc0, 0x0c, 0x00, 0x21, 0x00, 0x01, 0x00, 0x00,
    0x02, 0x43, 0x00, 0x1e, 0x00, 0x14, 0x00, 0x00,
    0x14, 0x66, 0x04, 0x61, 0x6c, 0x74, 0x33, 0x04,
    0x78, 0x6d, 0x70, 0x70, 0x01, 0x6c, 0x06, 0x67,
    0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x03, 0x63, 0x6f,
    0x6d, 0x00,
};
/* res_query("_xmpp-client._tcp.jabber.calyxinstitute.org", C_IN, T_SRV, ...) */
static const unsigned char data4[] = {
    0x8d, 0x58, 0x81, 0x80, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00,    // .X........
    0x00, 0x00, 0x0c, 0x5f, 0x78, 0x6d, 0x70, 0x70, 0x2d, 0x63,    // ..._xmpp-c
    0x6c, 0x69, 0x65, 0x6e, 0x74, 0x04, 0x5f, 0x74, 0x63, 0x70,    // lient._tcp
    0x06, 0x6a, 0x61, 0x62, 0x62, 0x65, 0x72, 0x0e, 0x63, 0x61,    // .jabber.ca
    0x6c, 0x79, 0x78, 0x69, 0x6e, 0x73, 0x74, 0x69, 0x74, 0x75,    // lyxinstitu
    0x74, 0x65, 0x03, 0x6f, 0x72, 0x67, 0x00, 0x00, 0x21, 0x00,    // te.org..!.
    0x01, 0xc0, 0x0c, 0x00, 0x21, 0x00, 0x01, 0x00, 0x00, 0x03,    // ....!.....
    0x83, 0x00, 0x1e, 0x00, 0x04, 0x00, 0x00, 0x14, 0x66, 0x10,    // ........f.
    0x69, 0x6a, 0x65, 0x65, 0x79, 0x6e, 0x72, 0x63, 0x36, 0x78,    // ijeeynrc6x
    0x32, 0x75, 0x79, 0x35, 0x6f, 0x62, 0x05, 0x6f, 0x6e, 0x69,    // 2uy5ob.oni
    0x6f, 0x6e, 0x00, 0xc0, 0x0c, 0x00, 0x21, 0x00, 0x01, 0x00,    // on....!...
    0x00, 0x03, 0x83, 0x00, 0x21, 0x00, 0x05, 0x00, 0x01, 0x14,    // ....!.....
    0x66, 0x06, 0x6a, 0x61, 0x62, 0x62, 0x65, 0x72, 0x0e, 0x63,    // f.jabber.c
    0x61, 0x6c, 0x79, 0x78, 0x69, 0x6e, 0x73, 0x74, 0x69, 0x74,    // alyxinstit
    0x75, 0x74, 0x65, 0x03, 0x6f, 0x72, 0x67, 0x00,                // ute.org.
};
/* res_query("_xmpp-client._tcp.jabber.org", C_IN, T_SRV, ...) with pointers */
static const unsigned char data5[] = {
    0x00, 0x00, 0x81, 0x80, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00,    // ..........
    0x00, 0x00, 0x0c, 0x5f, 0x78, 0x6d, 0x70, 0x70, 0x2d, 0x63,    // ..._xmpp-c
    0x6c, 0x69, 0x65, 0x6e, 0x74, 0x04, 0x5f, 0x74, 0x63, 0x70,    // lient._tcp
    0x06, 0x6a, 0x61, 0x62, 0x62, 0x65, 0x72, 0x03, 0x6f, 0x72,    // .jabber.or
    0x67, 0x00, 0x00, 0x21, 0x00, 0x01, 0xc0, 0x0c, 0x00, 0x21,    // g..!.....!
    0x00, 0x01, 0x00, 0x00, 0x01, 0x9e, 0x00, 0x12, 0x00, 0x1f,    // ..........
    0x00, 0x1e, 0x14, 0x66, 0x09, 0x68, 0x65, 0x72, 0x6d, 0x65,    // ...f.herme
    0x73, 0x32, 0x76, 0x36, 0xc0, 0x1e, 0xc0, 0x0c, 0x00, 0x21,    // s2v6.....!
    0x00, 0x01, 0x00, 0x00, 0x01, 0x9e, 0x00, 0x10, 0x00, 0x1e,    // ..........
    0x00, 0x1e, 0x14, 0x66, 0x07, 0x68, 0x65, 0x72, 0x6d, 0x65,    // ...f.herme
    0x73, 0x32, 0xc0, 0x1e,                                        // s2..
};
/* hacked data2 with two empty-string targets. */
static const unsigned char data6[] = {
    0xf2, 0x98, 0x81, 0x80, 0x00, 0x01, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x0c, 0x5f, 0x78, 0x6d,
    0x70, 0x70, 0x2d, 0x63, 0x6c, 0x69, 0x65, 0x6e,
    0x74, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x06, 0x6a,
    0x61, 0x62, 0x62, 0x65, 0x72, 0x03, 0x6f, 0x72,
    0x67, 0x00, 0x00, 0x21, 0x00, 0x01, 0xc0, 0x0c,
    0x00, 0x21, 0x00, 0x01, 0x00, 0x00, 0x03, 0x83,
    0x00, 0x07, 0x00, 0x1e, 0x00, 0x1e, 0x14, 0x66,
    0x00, 0xc0, 0x0c, 0x00, 0x21, 0x00, 0x01, 0x00,
    0x00, 0x03, 0x83, 0x00, 0x08, 0x00, 0x1f, 0x00,
    0x1e, 0x14, 0x66, 0xc0, 0x40,
};

static const struct {
    const unsigned char *data;
    size_t len;
    char *target;
    unsigned short port;
    int target_nr;
} tests[] = {
    {
        .data = data1,
        .len = sizeof(data1),
        .target = "jabber.kiev.ua",
        .port = 5222,
        .target_nr = 1,
    },
    {
        .data = data2,
        .len = sizeof(data2),
        .target = "hermes2.jabber.org",
        .port = 5222,
        .target_nr = 2,
    },
    {
        .data = data3,
        .len = sizeof(data3),
        .target = "xmpp.l.google.com",
        .port = 5222,
        .target_nr = 5,
    },
    {
        .data = data4,
        .len = sizeof(data4),
        .target = "ijeeynrc6x2uy5ob.onion",
        .port = 5222,
        .target_nr = 2,
    },
    {
        .data = data5,
        .len = sizeof(data5),
        .target = "hermes2.jabber.org",
        .port = 5222,
        .target_nr = 2,
    },
    {
        .data = data6,
        .len = sizeof(data6),
        .target = "",
        .port = 5222,
        .target_nr = 2,
    },
};

static int srv_rr_list_len(resolver_srv_rr_t *list)
{
    int nr;

    for (nr = 0; list != NULL; ++nr, list = list->next);

    return nr;
}

int main(int argc, char **argv)
{
    xmpp_ctx_t *ctx;
    xmpp_rand_t *rand;
    resolver_srv_rr_t *srv_rr_list;
    char *domain;
    unsigned char *buf;
    unsigned short port;
    size_t i;
    int ret;

    ctx = xmpp_ctx_new(NULL, NULL);
    assert(ctx != NULL);

    printf("resolver_srv_lookup_buf() tests.\n");
    for (i = 0; i < ARRAY_SIZE(tests); ++i) {
        printf("Test #%zu: ", i + 1);
        ret = resolver_srv_lookup_buf(ctx, tests[i].data, tests[i].len,
                                      &srv_rr_list);
        assert(ret == XMPP_DOMAIN_FOUND);
        assert(srv_rr_list != NULL);
        if (tests[i].target_nr != srv_rr_list_len(srv_rr_list)) {
            printf("fail! got %d targets, but should be %d\n",
                   srv_rr_list_len(srv_rr_list), tests[i].target_nr);
            return 1;
        }
        /* check only 1st result */
        domain = srv_rr_list->target;
        port = srv_rr_list->port;
        COMPARE(tests[i].target, domain);
        if (tests[i].port != port) {
            printf("fail! got port=%u, but should be %u\n",
                   (unsigned)port, (unsigned)tests[i].port);
            return 1;
        }
        printf("ok\n");
        resolver_srv_free(ctx, srv_rr_list);
    }

    /*
     * The next test case must not crash and is supposed to be checked
     * under valgrind.
     */
    printf("Test of a broken message: ");
    rand = xmpp_rand_new(ctx);
    assert(rand != NULL);
    assert(sizeof(data2) > 64);
    buf = xmpp_alloc(ctx, sizeof(data2));
    assert(buf != NULL);
    memcpy(buf, data2, 64);
    xmpp_rand_bytes(rand, &buf[64], sizeof(data2) - 64);
    ret = resolver_srv_lookup_buf(ctx, buf, sizeof(data2), &srv_rr_list);
    if (ret == XMPP_DOMAIN_FOUND && srv_rr_list != NULL)
        resolver_srv_free(ctx, srv_rr_list);
    xmpp_free(ctx, buf);
    xmpp_rand_free(ctx, rand);
    printf("ok\n");

    xmpp_ctx_free(ctx);

    return 0;
}
