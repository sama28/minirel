unsigned int bread_int(unsigned char* buffer,int size,int* init){
    unsigned int tmp=0,base=1;
    for (int i=0;i<size;i++){
        tmp=tmp+base*(int)buffer[i+*init];
		base=base*256;
        //printf("%02x\n",buffer[i+*init]);
    }
    //printf("%d\n",tmp);
    *init+=size;
    return tmp;
}