void bread_string(unsigned char* buffer,int size,int* init,unsigned char* tmp){
    for (int i=0;i<size;i++){
        tmp[i]=buffer[i+*init];
    }
    *init+=size;
    return ;
}