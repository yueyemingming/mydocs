
# libcurl代码示例

下载的libcurl中自带了很多示例代码，在docs\examples目录下，建议下载该库后好好阅读一下这些代码。

libcurl的API文档在docs\libcurl\index.html中。

## 1. 基本的http GET/POST操作

```c
#include <stdio.h>
#include <curl/curl.h>

bool getUrl(char *filename)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)    // 返回结果用文件存储
        return false;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: Agent-007");
    curl = curl_easy_init();                    // 初始化
    if (curl==nullptr) {
        fclose(fp);
        return false;
    }

    //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");   // 代理
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);            // 改协议头
    curl_easy_setopt(curl, CURLOPT_URL,"http://www.baidu.com");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);                  //将返回的http头输出到fp指向的文件
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp);                 //将返回的html主体数据输出到fp指向的文件
    res = curl_easy_perform(curl);                                  // 执行
    if (res != 0) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    fclose(fp);
    return true;
}

bool postUrl(char *filename)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)
        return false;
    curl = curl_easy_init();
    if (curl==nullptr) {
        fclose(fp);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt");                      // 指定cookie文件
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "&logintype=uid&u=xieyan&psw=xxx86");    // 指定post内容
    //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
    curl_easy_setopt(curl, CURLOPT_URL, " http://mail.sina.com.cn/cgi-bin/login.cgi "); // 指定url
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    res = curl_easy_perform(curl);                                                      // 执行
    if (res != 0) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    fclose(fp);
    return true;
}
int main(void)
{
    getUrl("/tmp/get.html");
    postUrl("/tmp/post.html");
}
```

编译运行

```bash
gcc get_post.c  -o get_post –lcurl
./get_post
```

## 2. 获取html网页

```c
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    CURL *curl;             //定义CURL类型的指针
    CURLcode res;           //定义CURLcode类型的变量，保存返回状态码
    if(argc!=2)  {
        printf("Usage : file <url>;\n");
        exit(1);
    }

    curl = curl_easy_init();        //初始化一个CURL类型的指针
    if(curl==nullptr) {
        return -1 ;
    }

    //设置curl选项. 其中CURLOPT_URL是让用户指 定url. argv[1]中存放的命令行传进来的网址
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
    //调用curl_easy_perform 执行我们的设置.并进行相关的操作. 在这 里只在屏幕上显示出来.
    res = curl_easy_perform(curl);
    //清除curl操作.
    curl_easy_cleanup(curl);
    return 0;
}
```

编译运行

```bash
gcc get_http.c  -o get_http –lcurl
./get_http www.baidu.com
```

## 3. 网页下载保存实例

```c
// 采用CURLOPT_WRITEFUNCTION 实现网页下载保存功能
#include <stdio.h>;
#include <stdlib.h>;
#include <unistd.h>;

#include <curl/curl.h>;
#include <curl/types.h>;
#include <curl/easy.h>;

FILE *fp;  //定义FILE类型指针
//这个函数是为了符合CURLOPT_WRITEFUNCTION而构造的
//完成数据保存功能
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)  
{
    int written = fwrite(ptr, size, nmemb, (FILE *)fp);
    return written;
}

int main(int argc, char *argv[])
{
    CURL *curl;

    curl_global_init(CURL_GLOBAL_ALL);  
    curl=curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);  

    if((fp=fopen(argv[2],"w"))==NULL)
    {
        curl_easy_cleanup(curl);
        exit(1);
    }

    //CURLOPT_WRITEFUNCTION 将后继的动作交给write_data函数处理
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);  
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    exit(0);
}
```

编译运行

```bash
gcc save_http.c -o save_http –lcurl
./save_http www.baidu.com
/tmp/baidu
```

## 4. 进度条实例显示文件下载进度

```c
//采用CURLOPT_NOPROGRESS， CURLOPT_PROGRESSFUNCTION    CURLOPT_PROGRESSDATA 实现文件传输进度提示功能
//函数采用了gtk库，故编译时需指定gtk库
//函数启动专门的线程用于显示gtk 进度条bar
#include <stdio.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <curl/types.h> /* new for v7 */
#include <curl/easy.h> /* new for v7 */

GtkWidget *Bar;
//这个函数是为了符合CURLOPT_WRITEFUNCTION而构造的
//完成数据保存功能
size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}
//这个函数是为了符合CURLOPT_READFUNCTION而构造的
//数据上传时使用
size_t my_read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fread(ptr, size, nmemb, stream);
}
//这个函数是为了符合CURLOPT_PROGRESSFUNCTION而构造的
//显示文件传输进度，t代表文件大小，d代表传 输已经完成部分
int my_progress_func(GtkWidget *bar,
                     double t, /* dltotal */
                     double d, /* dlnow */
                     double ultotal,
                     double ulnow)
{
    /*  printf("%d / %d (%g %%)\n", d, t, d*100.0/t);*/
    gdk_threads_enter();
    gtk_progress_set_value(GTK_PROGRESS(bar), d*100.0/t);
    gdk_threads_leave();
    return 0;
}

void *my_thread(void *ptr)
{
    CURL *curl;
    CURLcode res;
    FILE *outfile;
    gchar *url = ptr;

    curl = curl_easy_init();
    if(curl)
    {
        outfile = fopen("test.curl", "w");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, my_read_func);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, Bar);

        res = curl_easy_perform(curl);

        fclose(outfile);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return NULL;
}

int main(int argc, char **argv)
{
    GtkWidget *Window, *Frame, *Frame2;
    GtkAdjustment *adj;

    /* Must initialize libcurl before any threads are started */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Init thread */
    g_thread_init(NULL);

    gtk_init(&argc, &argv);
    Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    Frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(Frame), GTK_SHADOW_OUT);
    gtk_container_add(GTK_CONTAINER(Window), Frame);
    Frame2 = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(Frame2), GTK_SHADOW_IN);
    gtk_container_add(GTK_CONTAINER(Frame), Frame2);
    gtk_container_set_border_width(GTK_CONTAINER(Frame2), 5);
    adj = (GtkAdjustment*)gtk_adjustment_new(0, 0, 100, 0, 0, 0);
    Bar = gtk_progress_bar_new_with_adjustment(adj);
    gtk_container_add(GTK_CONTAINER(Frame2), Bar);
    gtk_widget_show_all(Window);

    if (!g_thread_create(&my_thread, argv[1], FALSE, NULL) != 0)
    g_warning("can't create the thread");


    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();
    return 0;
}
```

编译运行

```bash
export PKG_CONFIG_PATH=/usr/lib/pkgconfig/
gcc progress.c –o progress ` pkg-config --libs –cflags gtk+-2..0` -lcurl –lgthread-2.0
./progress  http://software.sky-union.cn/index.asp
```

## 5. 断点续传实例

```c
//采用CURLOPT_RESUME_FROM_LARGE 实现文件断点续传功能
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include <curl/curl.h>
//这个函数为CURLOPT_HEADERFUNCTION参数构造
/* 从http头部获取文件size*/
size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
    int r;
    long len = 0;

    /* _snscanf() is Win32 specific */
    // r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
    r = sscanf(ptr, "Content-Length: %ld\n", &len);
    if (r) /* Microsoft: we don't read the specs */
    *((long *) stream) = len;

    return size * nmemb;
}

/* 保存下载文件 */
size_t wirtefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
        return fwrite(ptr, size, nmemb, stream);
}

/*读取上传文件 */
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    FILE *f = stream;
    size_t n;

    if (ferror(f))
        return CURL_READFUNC_ABORT;

    n = fread(ptr, size, nmemb, f) * size;

    return n;
}

// 下载 或者上传文件函数
int download(CURL *curlhandle, const char * remotepath, const char * localpath,
           long timeout, long tries)
{
    FILE *f;
    curl_off_t local_file_len = -1 ;
    long filesize =0 ;

    CURLcode r = CURLE_GOT_NOTHING;
    int c;
    struct stat file_info;
    int use_resume = 0;
    /* 得到本地文件大小 */
    //if(access(localpath,F_OK) ==0)
  
    if(stat(localpath, &file_info) == 0)
    {
        local_file_len =  file_info.st_size;
        use_resume  = 1;
    }

    //采用追加方式打开文件，便于实现文件断点续传工作
    f = fopen(localpath, "ab+");
    if (f == NULL) {
        perror(NULL);
        return 0;
    }

    //curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
    curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);  // 设置连接超时，单位秒
    //设置http 头部处理函数
    curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
    curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
    // 设置文件续传的位置给libcurl
    curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, wirtefunc);

    //curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
    //curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
    curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
    r = curl_easy_perform(curlhandle);
    fclose(f);

    if (r == CURLE_OK)
        return 1;
    else {
        fprintf(stderr, "%s\n", curl_easy_strerror(r));
        return 0;
    }
}

int main(int c, char **argv) {
    CURL *curlhandle = NULL;

    curl_global_init(CURL_GLOBAL_ALL);
    curlhandle = curl_easy_init();

    //download(curlhandle, "ftp://user:pass@host/path/file", "C:\\file", 0, 3);
    download(curlhandle , "http://software.sky-union.cn/index.asp","/work/index.asp",1,3);
    curl_easy_cleanup(curlhandle);
    curl_global_cleanup();

    return 0;
}
```

编译运行

```bash
gcc resume.c -o resume –lcurl
./resume
```
