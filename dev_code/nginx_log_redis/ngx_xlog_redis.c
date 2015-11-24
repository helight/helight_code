/* ngx_log_redis_module.c  
 * for Redis)
 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_http.h>
#include <ngx_buf.h>

#include "credis.h"

REDIS redis = NULL;

typedef struct {
		ngx_str_t	xlog_redis;
} ngx_log_redis_loc_conf_t;

static ngx_int_t ngx_xlog_redis_filter_init(ngx_conf_t *cf);
static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt ngx_http_next_body_filter;

static void *ngx_log_redis_create_loc_conf(ngx_conf_t *cf);
static char *ngx_log_redis_merge_loc_conf(ngx_conf_t *cf,
				void *parent, void *child);

static ngx_command_t ngx_log_redis_commands[] = {
		{
			ngx_string("xlog_redis"),
			NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
			ngx_conf_set_str_slot,
			NGX_HTTP_LOC_CONF_OFFSET,
			offsetof(ngx_log_redis_loc_conf_t, xlog_redis),
			NULL
		},
		ngx_null_command
};

static ngx_http_module_t ngx_xlog_redis_module_ctx = {
	NULL,							/* preconfiguration */
	ngx_xlog_redis_filter_init,      /* postconfiguration */
	NULL,                           /* create main configuration */
	NULL,                           /* init main configuration */
	NULL,                           /* create server configuration */
	NULL,                           /* merge server configuration */
	ngx_log_redis_create_loc_conf,	/* create location configuration */
	ngx_log_redis_merge_loc_conf	/* merge location configuration */
};

ngx_module_t ngx_xlog_redis = {
	NGX_MODULE_V1,
	&ngx_xlog_redis_module_ctx,      /* module context */ 
	ngx_log_redis_commands,	         /* module directives */ 
	NGX_HTTP_MODULE,                 /* module type */
	NULL,                            /* init master */
	NULL,                            /* init module */
	NULL,                            /* init process */
	NULL,                            /* init thread */
	NULL,                            /* exit thread */
	NULL,                            /* exit process */
	NULL,                            /* exit master */
	NGX_MODULE_V1_PADDING
};

static ngx_int_t 
ngx_xlog_redis_header_filter(ngx_http_request_t *r)
{
	ngx_log_redis_loc_conf_t  *conf;
	int rc = 0;
	char ttm[32];

    conf = (ngx_log_redis_loc_conf_t  *)ngx_http_get_module_loc_conf
		(r, ngx_xlog_redis);
	if (conf == NULL)
		return ngx_http_next_header_filter(r);
		
	if (redis == NULL) {
		redis = credis_connect(NULL, 0, 10000);
	
		if (redis == NULL) {
			return ngx_http_next_header_filter(r); 
		}
	}
	ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, 
			"ngx_log_redis_header_filter:%s", conf->xlog_redis.data);
	snprintf(ttm, sizeof(ttm), "%d", (int)time(NULL));	
	rc = credis_lpush(redis, "xlog_redis", ttm);	
	//rc = credis_hset();
	rc = rc;
	if (r != r->main) {
		return ngx_http_next_header_filter(r);  
	} 

	r->headers_out.content_length_n += sizeof("Nginx-HelightXu\n") - 1;  
	credis_close(redis);
	
	return ngx_http_next_header_filter(r);  
}

static ngx_int_t   
ngx_xlog_redis_body_filter(ngx_http_request_t *r, ngx_chain_t *in)  
{  
	if (r != r->main) {
		return ngx_http_next_body_filter(r, in);  
	}  
	ngx_chain_t *chain_link;  
	int chain_contains_last_buffer = 0;  
	for ( chain_link = in; chain_link != NULL; chain_link = chain_link->next ) {  
			if (chain_link->buf->last_buf) {  
					chain_contains_last_buffer = 1;  
					break;  
			}  
	}  
	if (!chain_contains_last_buffer)  
			return ngx_http_next_body_filter(r, in);  
	ngx_buf_t    *b;  
	b = ngx_calloc_buf(r->pool);  
	if (b == NULL) {  
			return NGX_ERROR;  
	}  
	b->pos = (u_char *) "Nginx-HelightXu\n";  
	b->last = b->pos + sizeof("Nginx-HelightXu\n") - 1;  
	b->memory = 1;  
	b->last_buf = 1;  
	ngx_chain_t   *added_link;  
	added_link = ngx_alloc_chain_link(r->pool);  
	if (added_link == NULL)  
			return NGX_ERROR;  
	added_link->buf = b;  
	added_link->next = NULL;  
	chain_link->next = added_link;  
	chain_link->buf->last_buf = 0;  
	added_link->buf->last_buf = 1;

    return ngx_http_next_body_filter(r, in);  
}

static ngx_int_t
ngx_xlog_redis_filter_init(ngx_conf_t *cf)
{
	ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_xlog_redis_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_xlog_redis_body_filter;
    
    return NGX_OK;
}

static void *ngx_log_redis_create_loc_conf(ngx_conf_t *cf)
{
	ngx_log_redis_loc_conf_t * conf;

	conf = (ngx_log_redis_loc_conf_t *)
			ngx_pcalloc(cf->pool, sizeof(ngx_log_redis_loc_conf_t));
	if (conf == NULL) {
		return NGX_CONF_ERROR;
	}
	conf->xlog_redis.len = 0;
	conf->xlog_redis.data = NULL;
	
	return conf;
}

static char *ngx_log_redis_merge_loc_conf(ngx_conf_t *cf,
				void *parent, void *child)
{
	ngx_log_redis_loc_conf_t *prev = (ngx_log_redis_loc_conf_t *)parent;
	ngx_log_redis_loc_conf_t *conf = (ngx_log_redis_loc_conf_t *)child;

	ngx_conf_merge_str_value(conf->xlog_redis, prev->xlog_redis, "");

	return NGX_CONF_OK;
}
