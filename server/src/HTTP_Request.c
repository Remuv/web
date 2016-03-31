/* $CORTO_GENERATED
 *
 * HTTP_Request.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

/* $header() */

#include "mongoose.h"

/* $end */

corto_rbtree _server_HTTP_Request_getGetForm(
    server_HTTP_Request* this)
{
/* $begin(corto/web/server/HTTP/Request/getGetForm) */
    struct mg_connection* conn = (struct mg_connection *)this->conn;
    size_t len = strlen(conn->query_string);
    char* buffer = corto_alloc(len + 1);
    if (buffer == NULL) {
        goto error;
    }
    strncpy(buffer, conn->query_string, len);
    buffer[len + 1] = '\0';
    return server_queryToMap(buffer);
error:
    return NULL;
/* $end */
}

corto_rbtree _server_HTTP_Request_getPostForm(
    server_HTTP_Request* this)
{
/* $begin(corto/web/server/HTTP/Request/getPostForm) */
    struct mg_connection* conn = (struct mg_connection *)this->conn;
    char* buffer = corto_alloc(conn->content_len + 1);
    if (buffer == NULL) {
        goto error;
    }
    strncpy(buffer, conn->content, conn->content_len);
    buffer[conn->content_len + 1] = '\0';
    return server_queryToMap(buffer);
error:
    return NULL;
/* $end */
}

corto_string _server_HTTP_Request_getVar(
    server_HTTP_Request* this,
    corto_string id)
{
/* $begin(corto/web/server/HTTP/Request/getVar) */
    static char value[256];

    mg_get_var((struct mg_connection *)this->conn, id, value, sizeof(value));

    return value;
/* $end */
}

corto_void _server_HTTP_Request_reply(
    server_HTTP_Request* this,
    corto_string msg)
{
/* $begin(corto/web/server/HTTP/Request/reply) */

    mg_printf_data((struct mg_connection *)this->conn, "%s", msg);

/* $end */
}

corto_void _server_HTTP_Request_sendfile(
    server_HTTP_Request* this,
    corto_string file)
{
/* $begin(corto/web/server/HTTP/Request/sendfile) */

    mg_send_file((struct mg_connection *)this->conn, file, "");
    this->file = TRUE;

/* $end */
}

corto_void _server_HTTP_Request_setHeader(
    server_HTTP_Request* this,
    corto_string name,
    corto_string val)
{
/* $begin(corto/web/server/HTTP/Request/setHeader) */

    mg_send_header((struct mg_connection *)this->conn, name, val);

/* $end */
}

corto_void _server_HTTP_Request_setStatus(
    server_HTTP_Request* this,
    corto_uint16 status)
{
/* $begin(corto/web/server/HTTP/Request/setStatus) */

    mg_send_status((struct mg_connection *)this->conn, status);

/* $end */
}
