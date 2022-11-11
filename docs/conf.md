# `.conf` documentation

```asm
server {
        listen      8080;  #hostname/IP
        server_name le_site.com www.le_site.com;
        location / {
            root    www;
            index   index.html;
        }
        client_max_body_size    10M;
        error_page  404 www/page_not_found.html;
        cgi cgi_files;
}
```

## NGINX

| TYPE                                                                                                    | DESCRIPTION                                                                      | FORMAT                        | EXAMPLE                                   |
|---------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------|-------------------------------|-------------------------------------------|
| [`listen`](https://nginx.org/en/docs/http/ngx_http_core_module.html#listen)                             | indicates the IP and TCP that the sever should listen on.                        | `listen [address]`            | `listen 8080`                             |
| [`server_name`](https://nginx.org/en/docs/http/ngx_http_core_module.html#server_name)                   | _Optional:_ allows multiple domains to be served from a single IP address.       | `server_name [name] ...`      | `server_name example.com www.example.com` |
| [`root`](https://nginx.org/en/docs/http/ngx_http_core_module.html#root)                                 | sets the root directory for requests                                             | `root [path]`                 | `root www`                                |
| [`client_max_body_size`](https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size) | Sets the maximum allowed size of the client request body                         | `client_max_body_size [size]` | `client_max_body_size 10M`                |
| [ `error_page`](https://nginx.org/en/docs/http/ngx_http_core_module.html#error_page)                    | Defines URI that will be shown for the specified errors.                         | `error_page [code] [uri]`     | `error_page 404 www/file_not_found.html`  |
| [`location`](https://nginx.org/en/docs/http/ngx_http_core_module.html#location)                         | Sets up the way the server responds to requests for resources within the server. | `location [uri]{ ... }`       | `location / { ... }`                      |
| -----> `index`                                                                                          | In location.  The file location sends to.                                        | `index [file_name]`           | `index index.html`                        |

## Ours


| TYPE  | DESCRIPTION           | FORMAT            | TYPE             |
|-------|-----------------------|-------------------|------------------|
| `cgi` | location of cgi files | `cgi [directory]` | `cgi /cgi_files` |

