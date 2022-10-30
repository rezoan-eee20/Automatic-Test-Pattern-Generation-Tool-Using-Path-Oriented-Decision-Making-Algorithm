#include "input.h"
/***************************************************************************************************
Insert an element "X" at end of LIST "Cur", if "X" is not already in "Cur". 
If the "Cur" is Null, it creates a single element List using "X"
***************************************************************************************************/
void InsertList(LIST **Cur,int X)
{
LIST *tl=NULL;
LIST *nl=NULL;

if ((tl=(LIST *) malloc(sizeof(LIST)))==NULL){ 
  printf("LIST: Out of memory\n");  
  exit(1);  }   
else{
  tl->Id=X;  tl->Next=NULL;
  if(*Cur==NULL){  
    *Cur=tl; 
    return;  }
  nl=*Cur; 
  while(nl!=NULL){
    if(nl->Id==X){       break;       }
    if(nl->Next==NULL){  nl->Next=tl; } 
    nl=nl->Next; }  
 }
return;
}//end of InsertEle
/***************************************************************************************************
Delete an element "X" from LIST "Cur",
***************************************************************************************************/	
void DeleteEle(LIST **Cur,int X)
{
LIST *fir = (*Cur);
LIST *lst = NULL;

if (fir==NULL) return;  
while((fir->Id!=X)&&(fir!= NULL)){
  lst=fir;
  fir=fir->Next; }   
if(fir!=NULL){
   if(lst==NULL){  (*Cur)=(*Cur)->Next;       }
   else{           lst->Next = fir->Next; }
   free(fir); }
else{ 
   return; }
return;
}//end of DeleteEle
/***************************************************************************************************************************
Return 1 if the element "x" is present in LIST "Cur"; Otherwise return  0  
*****************************************************************************************************************************/
int FindEle(LIST *Cur,int X)
{
LIST *temp=NULL;
	
temp=Cur;
while(temp!=NULL){
  if(temp->Id==X) return 1;
  temp=temp->Next; }
return 0;
}//end of FindEle
/***************************************************************************************************
Print the elements in LIST "Cur"          
***************************************************************************************************/
void PrintList(LIST *Cur)
{
LIST *tmp=Cur;

while(tmp!=NULL){   
  printf("%d  ", tmp->Id);
  tmp = tmp->Next; } 
return;
}//end of PrintList
/***************************************************************************************************
Count the total number  of elements in LIST "Cur"          
***************************************************************************************************/
int CountList(LIST *Cur)
{
LIST *tmp=Cur;
int size=0;

while(tmp!=NULL){   
  size++;
  tmp = tmp->Next; } 
return size;
}//end of PrintList
/***************************************************************************************************
Free all elements in  LIST "Cur"  
***************************************************************************************************/
void FreeList(LIST **Cur)
{
LIST *tmp=NULL;

if(*Cur==NULL){   return;  }
tmp=(*Cur);
while((*Cur) != NULL){
  tmp=tmp->Next;
  free(*Cur); 
  (*Cur)=tmp; }   
(*Cur)=NULL;
return;
}//end of FreeList 
/***************************************************************************************************
Initialize the paricular member of GATE structure
***************************************************************************************************/        
void InitializeCircuit(GATE *Node,int Num)
{
Node[Num].Name=(char *) malloc(Mnam *sizeof(char));           //Dynamic memory allocation for an array
bzero(Node[Num].Name,Mnam);                                   //Clearing the string
Node[Num].Type=Node[Num].Nfi=Node[Num].Nfo=Node[Num].Mark=0;
Node[Num].Val=6;  
Node[Num].Fin=Node[Num].Fot=NULL;
// Node[Num].Rpath=Node[Num].Fpath=NULL;   
return;
}//end of InitiGat
/***************************************************************************************************
Print all contents(attribute) of all active member of GATE structure(DdNodes are not printed)
***************************************************************************************************/
void PrintGats(GATE *Node,int Tgat)
{
int i;

printf("\nId\tName\tType\t#In\t#Out\tMark\tValue\t\tFanin\t\tFanout");
for(i=1;i<=Tgat;i++){
  if(Node[i].Type!=0){
    printf("\n%d\t%s\t%d\t%d\t%d\t%d\t%d\t\t",i,Node[i].Name,Node[i].Type,Node[i].Nfi,Node[i].Nfo,Node[i].Mark,Node[i].Val);
    PrintList(Node[i].Fin);  printf("\t\t");
    PrintList(Node[i].Fot); }}
return;
}//end of PrintGats
/***************************************************************************************************
Free the memory of all contents of all members of GATE structure(DdNodes are already cleared)
***************************************************************************************************/
void ClearGat(GATE *Node,int Tgat)
{
int i;

for(i=1;i<=Tgat;i++){ 
  free(Node[i].Name);
  Node[i].Type=Node[i].Nfi=Node[i].Nfo=Node[i].Mark=Node[i].Val=0;
  FreeList(&Node[i].Fin);  FreeList(&Node[i].Fot); }
return;
}//end of ClearGat
/***************************************************************************************************
Count the Total Number of Primary inputs and outputs
***************************************************************************************************/
void CountPri(GATE *Node,int Tgat,int *Npi,int *Npo)
{
int i,j,k;

i=j=k=0;
for(i=1;i<=Tgat;i++){ 
  if(Node[i].Type!=0){
    if(Node[i].Nfi==0){      j++; }
    if(Node[i].Nfo==0){      k++; } }}
*Npi=j; *Npo=k;
return;
}//end of CountPri
/***************************************************************************************************
Convert (char *) type read to (int)     
***************************************************************************************************/
int AssignType(char *Gtyp)
{
if      ((strcmp(Gtyp,"inpt")==0) || (strcmp(Gtyp,"INPT")==0))       return 1;
else if ((strcmp(Gtyp,"from")==0) || (strcmp(Gtyp,"FROM")==0))       return 2;
else if ((strcmp(Gtyp,"buff")==0) || (strcmp(Gtyp,"BUFF")==0))       return 3;
else if ((strcmp(Gtyp,"not")==0)  || (strcmp(Gtyp,"NOT")==0))        return 4;
else if ((strcmp(Gtyp,"and")==0)  || (strcmp(Gtyp,"AND")==0))        return 5;
else if ((strcmp(Gtyp,"nand")==0) || (strcmp(Gtyp,"NAND")==0))       return 6;
else if ((strcmp(Gtyp,"or")==0)   || (strcmp(Gtyp,"OR")==0))         return 7;
else if ((strcmp(Gtyp,"nor")==0)  || (strcmp(Gtyp,"NOR")==0))        return 8;
else if ((strcmp(Gtyp,"xor")==0)  || (strcmp(Gtyp,"XOR")==0))        return 9;
else if ((strcmp(Gtyp,"xnor")==0) || (strcmp(Gtyp,"XNOR")==0))       return 10;
else                          			                     return 0;
}//end of AssignType
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
		strcpy(Node[id].Name,name);
		Node[id].Type=AssignType(noty);
		//fill in fanin and fanout numbers
		if(Node[id].Type!=FROM) {   fout= atoi(str1);  fin=atoi(str2); }
		else{                       fin=fout= 1; strcpy(from,str1);    }
		if(id > mid){ mid=id;  }
		Node[id].Nfo=fout;  Node[id].Nfi=fin;
		if(fout==0){  Node[id].Po=1; }
		//create fanin and fanout lists
		switch (Node[id].Type)  {
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
				InsertList(&Node[id].Fin,fid);
				InsertList(&Node[fid].Fot,id); }
			fscanf(Isc,"\n");  break;
		case FROM  : for(i=mid;i>0;i--){
				if(Node[i].Type!=0){
					if(strcmp(Node[i].Name,from)==0){  fid=i; break; } } }
			InsertList(&Node[id].Fin,fid);
			InsertList(&Node[fid].Fot,id);   break;
		} //end case
		bzero(line,strlen(line));
		fgets(line,Mlin,Isc);
	} // end while
	return mid;
}//end of ReadIsc 
/****************************************************************************************************************************/
