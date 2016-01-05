/* $CORTO_GENERATED
 *
 * REST.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

/* $header() */
#include "json.h"

void server_REST_apiRequest(
    server_REST this,
    server_HTTP_Connection c,
    server_HTTP_Request *r)
{
    corto_bool value = FALSE;
    corto_bool meta = FALSE;
    corto_ll items = corto_llNew();
    corto_string reply = NULL;
    corto_bool multiple = FALSE;

    /* Set correct content type */
    server_HTTP_Request_setHeader(
        r,
        "Content-Type",
        "application/json; charset=utf-8");

    /* Determine what to show */
    if (!strcmp(server_HTTP_Request_getVar(r, "value"), "true")) { value = TRUE; }
    if (!strcmp(server_HTTP_Request_getVar(r, "meta"), "true")) { meta = TRUE; }

    /* Select objects with URI */
    corto_iter iter;
    corto_object scope;
    corto_string select = server_HTTP_Request_getVar(r, "select");
    if (!strlen(select)) {
        select = r->uri + strlen(this->prefix);
        scope = NULL;
    } else {
        multiple = (strchr(select, '*') != NULL);
        scope = corto_resolve(NULL, r->uri + strlen(this->prefix));
    }
    if (corto_select(scope, select, &iter)) {
        server_HTTP_Request_setStatus(r, 400);
        server_HTTP_Request_reply(r, "400: bad request\n");
        return;
    }

    if (corto_selectContentType(&iter, "text/json")) {
        corto_string msg;
        server_HTTP_Request_setStatus(r, 500);
        corto_asprintf(&msg,
            "500: failed to set contentType to text/json\n"
            "    details: %s\n", corto_lasterr());
        server_HTTP_Request_reply(r, msg);
        corto_dealloc(msg);
        return;
    }

    /* Add object to result list */
    corto_resultIterForeach(iter, result) {
        corto_string metaTxt = NULL;
        corto_string valueTxt = NULL;
        corto_string item = NULL;
        if (meta) {
            corto_asprintf(
              &metaTxt,
              "\"meta\":{\"type\":\"%s\"}",
              result.type
            );
        }
        if (value) {
            valueTxt = corto_result_getText(&result);
        }

        {
            corto_id id; sprintf(id, "%s/%s", result.parent, result.name);
            corto_cleanpath(id);
            corto_asprintf(
                &item,
                "{\"_id\":\"%s\"%s%s%s%s%s}",
                id,
                metaTxt ? ", " : "",
                metaTxt ? metaTxt : "",
                valueTxt ? ", " : "",
                valueTxt ? "\"value\":" : "",
                valueTxt ? valueTxt : ""
            );
            corto_dealloc(metaTxt);
            corto_llAppend(items, item);
        }
    }

    if (!corto_llSize(items)) {
        if (multiple) {
            server_HTTP_Request_reply(r, "[]");
            return;
        } else {
            server_HTTP_Request_setStatus(r, 404);
            server_HTTP_Request_reply(r, "404: resource not found");
            return;
        }
    }

    if (multiple) {
        reply = corto_strdup("[");
    }

    /* Merge items into one result string */
    corto_int32 count = 0;
    iter = corto_llIter(items);
    while (corto_iterHasNext(&iter)) {
        corto_string item = corto_iterNext(&iter);
        corto_string temp = NULL;

        /* Wildly inefficient */
        if (reply) {
            if (!count) {
                corto_asprintf(
                    &temp,
                    "%s%s",
                    reply,
                    item);
            } else {
                corto_asprintf(
                    &temp,
                    "%s, %s",
                    reply,
                    item);
            }
            corto_dealloc(reply);
            reply = temp;
        } else {
            reply = corto_strdup(item);
        }
        count++;
    }


    if (multiple) {
        corto_string temp = NULL;
        corto_asprintf(
            &temp,
            "%s]",
            reply);
        corto_dealloc(reply);
        reply = temp;
    }

    server_HTTP_Request_reply(r, reply);
}
/* $end */

corto_int16 _server_REST_construct(server_REST this) {
/* $begin(corto/web/server/REST/construct) */

    if (!this->prefix) {
        corto_setstr(&this->prefix, "");
    }

    return server_Service_construct(this);

/* $end */
}

corto_int16 _server_REST_onRequest(server_REST this, server_HTTP_Connection c, server_HTTP_Request *r) {
/* $begin(corto/web/server/REST/onRequest) */
    CORTO_UNUSED(this);

    if (!strlen(this->prefix) || !memcmp(r->uri, this->prefix, strlen(this->prefix))) {
        server_REST_apiRequest(this, c, r);
        return 1;
    } else {
        return 0;
    }

/* $end */
}