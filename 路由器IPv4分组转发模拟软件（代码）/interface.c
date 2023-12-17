#include<gtk/gtk.h>

GtkWidget *file;
GtkWidget *pcapFile;
GtkWidget *routeFile;

void file_OK(GtkWidget *w,gpointer data)
{
	gtk_entry_set_text(GTK_ENTRY(data),gtk_file_selection_get_filename(GTK_FILE_SELECTION(file)));

	gtk_widget_hide_all(file);
}
//查找文件
void select_file(GtkWidget *w,gpointer data)
{
        

	file = gtk_file_selection_new("File selection");
	g_signal_connect(G_OBJECT(file),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(file)->ok_button),"clicked",G_CALLBACK(file_OK),data);
	g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(file)->cancel_button),"clicked",G_CALLBACK(gtk_widget_hide_all),file);

        gtk_widget_show(file);
}

char fileRoute[1024];
char filePcap[1024];

char * copyStr(char *dest, const gchar *src)
{
	if(src == NULL){
		return NULL;	
	}
	char *strDest = dest;
	while((*dest++ = *src++) != '\0');
	return strDest;
}

void on_button_clicked(GtkWidget *button,gpointer data)  
{  
	if((int)data == 1){  
		gtk_entry_set_text(GTK_ENTRY(pcapFile),"");  
		gtk_entry_set_text(GTK_ENTRY(routeFile),"");  
	}
	else if((int)data == 2){  
		const gchar* pcapname = gtk_entry_get_text(GTK_ENTRY(pcapFile));  
	        const gchar* routename = gtk_entry_get_text(GTK_ENTRY(routeFile));  
	        g_print("读数据:%s\n",pcapname);
		g_print("file-name:%s\n",pcapname);
		g_print("route-file-name:%s\n",routename);

		g_print("拷贝字符串\n");
		copyStr(fileRoute, routename);
		copyStr(filePcap, pcapname);

		printf("fileRoute:%s,filePcap:%s\n",fileRoute,filePcap);	

		read_Route(fileRoute);
		read_pcap(filePcap);
	} 
}  


int main(int argc, char *argv[])
{
	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *window;

	gtk_init(&argc,&argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"模拟IPv4转发软件");

	table = gtk_table_new(4,8,TRUE);
	label = gtk_label_new("选择文件,完成分组转发");
	gtk_table_attach_defaults(GTK_TABLE(table),label,3,5,0,1);

	//pcap文件路径选择
	label = gtk_label_new("pcap文件路径:");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,2,1,2);

	pcapFile = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),pcapFile,2,6,1,2);
	button = gtk_button_new_with_label("选择文件");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(select_file),pcapFile);
	gtk_table_attach_defaults(GTK_TABLE(table),button,6,7,1,2);

	//存储路由表的文件路径选择
	label = gtk_label_new("路由表文件路径:");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,2,2,3);

	routeFile = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),routeFile,2,6,2,3);
	button = gtk_button_new_with_label("选择文件");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(select_file),routeFile);
	gtk_table_attach_defaults(GTK_TABLE(table),button,6,7,2,3);


	button = gtk_button_new_with_label("清空");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_button_clicked),(gpointer)1);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,3,3,4);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("确认");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_button_clicked),(gpointer)2);
	g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),window);
	gtk_table_attach_defaults(GTK_TABLE(table),button,5,7,3,4);
	gtk_widget_show(button);


	gtk_container_add(GTK_CONTAINER(window),table);
	gtk_widget_show_all(window);
	gtk_main();

	return FALSE;

}
