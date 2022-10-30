/*****************************************************************************************************************************
 Routine to read the Bench Mark(.isc files)
*****************************************************************************************************************************/
int ReadIsc(FILE *Isc,GATE *Node)
{
	char c,noty[Mlin],from[Mlin],str1[Mlin],str2[Mlin],name[Mlin],line[Mlin];
	int  i,id,fid,fin,fout,mid=0;

	// intialize all nodes in Node structure
	for(i=0;i<Mnod;i++){ InitializeCircuit(Node,i); }
	//skip the comment lines
	do
		fgets(line,Mlin,Isc);
	while(line[0] == '*');
	// read line by line
	while(!feof(Isc)){
		//initialize temporary strings
		bzero(noty,strlen(noty));    bzero(from,strlen(from));
		bzero(str1,strlen(str1));    bzero(str2,strlen(str2));
		bzero(name,strlen(name));
		//break line into data
		sscanf(line, "%d %s %s %s %s",&id,name,noty,str1,str2);
		//fill in the type
		strcpy(Node[id].nam,name);
		Node[id].typ=AssignType(noty);
		//fill in fanin and fanout numbers
		if(Node[id].typ!=FROM) {   fout= atoi(str1);  fin=atoi(str2); }
		else{                       fin=fout= 1; strcpy(from,str1);    }
		if(id > mid){ mid=id;  }
		Node[id].nfo=fout;  Node[id].nfi=fin;
		if(fout==0){  Node[id].po=1; }
		//create fanin and fanout lists
		switch (Node[id].typ)  {
		case 0     : printf("ReadIsc: Error in input file (Node %d)\n",id); exit(1);
		case INPT  : break;
		case AND   :
		case NAND  :
		case OR    :
		case NOR   :
		case XOR   :
		case XNOR  :
		case BUFF  :
		case NOT   : for(i=1;i<=fin;i++) {
				fscanf(Isc, "%d", &fid);
				InsertList(&Node[id].fin,fid);
				InsertList(&Node[fid].fot,id); }
			fscanf(Isc,"\n");  break;
		case FROM  : for(i=mid;i>0;i--){
				if(Node[i].typ!=0){
					if(strcmp(Node[i].nam,from)==0){  fid=i; break; } } }
			InsertList(&Node[id].fin,fid);
			InsertList(&Node[fid].fot,id);   break;
		} //end case
		bzero(line,strlen(line));
		fgets(line,Mlin,Isc);
	} // end while
	return mid;
}//end of ReadIsc 
