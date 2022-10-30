#include <time.h>
#include "input.h"
/***************************************************************************************************************************
Truth Table
****************************************************************************************************************************/
//clock_t start_t, end_t, total_t;
   	
//start_t = clock();


int NOTG[5] = {1,0,DC,Db,D};
int ANDG[5][5]={{0,0,0,0,0},
		{0,1,DC,D,Db},
		{0,DC,DC,DC,DC},
		{0,D,DC,D,0},
		{0,Db,DC,0,Db}};
int ORG[5][5]={{0,1,DC,D,Db},
               {1,1,1,1,1},
               {DC,1,DC,DC,DC},
               {D,1,DC,D,1},
               {Db,1,DC,1,Db}};


int ReadPat(FILE *Pat, PATTERN *Patterns){
	char line[Mlin];// variable line with mlin max characters
	int i=0,n; // i=row ,n =column
	// read line by line
	while(!feof(Pat)){
		if(fgets(line,Mlin,Pat)!=NULL){//reads the characters in the stream
			for(n=0;n<strlen(line);n++){//strlen=measures the string length
				if(line[n]=='1')			Patterns[i].Pat[n]=1;
				else if(line[n]=='0')			Patterns[i].Pat[n]=0;
				else if(line[n]=='x'||line[n]=='X')	Patterns[i].Pat[n]=DC;
			}
			i++;
			if(i==100) break;
		}
	}
	return i; //total number of patterns
}

void LogicSim(GATE *Node, PATTERN *Patterns, int Tgat, int Tpat, FILE *Res)
{
	int i,j,k; //i=total patterns,j=total gates

	for(i=0;i<Tpat;i++){
		k=0;
		for(j=0;j<=Tgat;j++){//printf("%d",j);getchar();
			if(Node[j].Type !=0){
				if(Node[j].Type==INPT){
					Node[j].Val = Patterns[i].Pat[k];
					k++;
				}
				else{
					Gout(Node, j);
				}
				if(Node[j].Po) {
					printf("%d",Node[j].Val);
					fputc(Node[j].Val+'0' , Res);// write the char c into file res,converting int to char
				}
			}
		}//PrintGats(Node,Tgat); 
		printf(" %d\n", i);
		fputc('\n' , Res);
		
	}
}

void Gout(GATE *Node, int a)
{
	LIST *lfin;
	int val; // local variable
	if(Node[a].Type==FROM){
		lfin=Node[a].Fin;
		while(lfin!=NULL){
			Node[a].Val=Node[lfin->Id].Val; 
			lfin=lfin->Next;
		}
	}
	else if(Node[a].Type==BUFF){
		lfin=Node[a].Fin;
		while(lfin!=NULL){
			Node[a].Val=Node[lfin->Id].Val; 
			lfin=lfin->Next;
		}
	}
	else if(Node[a].Type==NOT){
		lfin=Node[a].Fin;
		while(lfin!=NULL){
			Node[a].Val=NOTG[Node[lfin->Id].Val]; 
			lfin=lfin->Next;
		}
	}
	else if(Node[a].Type==AND){
		lfin=Node[a].Fin;
		val=Node[lfin->Id].Val;// non control for AND gate
		lfin=lfin->Next;
		while(lfin!=NULL){
			val=ANDG[val][Node[lfin->Id].Val]; 
			lfin=lfin->Next;
		}
		Node[a].Val=val;
	}
	else if(Node[a].Type==NAND){
		lfin=Node[a].Fin;
		val=Node[lfin->Id].Val;
		lfin=lfin->Next;// non control for AND gate
		while(lfin!=NULL){
			val=ANDG[val][Node[lfin->Id].Val]; 
			lfin=lfin->Next;
		}
		Node[a].Val=NOTG[val];
	}
	else if(Node[a].Type==OR){
		lfin=Node[a].Fin;
		val=Node[lfin->Id].Val;// non control for OR gate
		lfin=lfin->Next;lfin=Node[a].Fin;
		while(lfin!=NULL){
			Node[a].Val=Node[lfin->Id].Val; 
			lfin=lfin->Next;
		}
		while(lfin!=NULL){
			val=ORG[val][Node[lfin->Id].Val]; 
			lfin=lfin->Next;
		 }
		Node[a].Val=val;
	}
	else if(Node[a].Type==NOR){
		lfin=Node[a].Fin;
		val=Node[lfin->Id].Val;// non control for OR gate
		lfin=lfin->Next;
		while(lfin!=NULL){
			val=ORG[val][Node[lfin->Id].Val]; 
			lfin=lfin->Next;
		 }
		Node[a].Val=NOTG[val];
	}
	else if(Node[a].Type==XOR) {printf("ERROR: XOR GATE FOUND\n");exit(0);}
	else if(Node[a].Type==XNOR){printf("ERROR: XNOR GATE FOUND\n");exit(0);}
}

/***************************************************************************************************/
//part2. PODEM
void resetckt(GATE *Node, int Tgat) // intialising the pI to dc
{
	
	int j;//j=total number of gates
	for(j=0;j<=Tgat;j++){
		if(Node[j].Type !=0){
			Node[j].Val=DC;
		}
	}
}

/*****************/
void logicSimImply(GATE *Node,int pi,int u, int *detected, int *faultmask, FAULT flt, LIST **Dfrontier)//g=input gate number
{
	LIST *lFot, *nxtnode,*lfin;
	int preVal;
	
	*detected=0;
	*faultmask=0;
	nxtnode=NULL;
	Node[pi].Val=u;
	if(Node[pi].Val==1 && flt.ftype==0 && pi==flt.Fid)
		Node[pi].Val=D;
	else if(Node[pi].Val==0 && flt.ftype==1 && pi==flt.Fid)
		Node[pi].Val=Db;
	else if(pi==flt.Fid && Node[pi].Val!=DC)
		*faultmask=1;
	//printf("pi=%d,g=%d,saf=%d,u=%d,val=%d", pi, flt.Fid,flt.ftype,u,Node[pi].Val); getchar();
	lFot=Node[pi].Fot;
	while(lFot!=NULL){
		InsertList(&nxtnode,lFot->Id);
		lFot=lFot->Next;
	}
	while(nxtnode!=NULL){
		preVal = Node[nxtnode->Id].Val;
		Gout(Node, nxtnode->Id);// logicsimulation result for a gate

		if(Node[nxtnode->Id].Val==1 && flt.ftype==0 && nxtnode->Id==flt.Fid)
			Node[nxtnode->Id].Val=D;
		else if(Node[nxtnode->Id].Val==0 && flt.ftype==1 && nxtnode->Id==flt.Fid)
			Node[nxtnode->Id].Val=Db;
		else if(nxtnode->Id==flt.Fid && Node[nxtnode->Id].Val !=DC)
			*faultmask=1;

		lfin=Node[nxtnode->Id].Fin;
		if (Node[nxtnode->Id].Val==DC){ 
			while(lfin!=NULL){
			 	if(Node[lfin->Id].Val==D || Node[lfin->Id].Val==Db ){
					InsertList(Dfrontier, nxtnode->Id);
					break;
				}
				 lfin=lfin->Next;
			}
		}
		if(FindEle(*Dfrontier,nxtnode->Id) && (lfin==NULL || Node[nxtnode->Id].Val!=DC))
			DeleteEle(Dfrontier,nxtnode->Id);

		if(Node[nxtnode->Id].Val!= preVal){ 
			lFot=Node[nxtnode->Id].Fot;
		}
		while(lFot!=NULL){
			InsertList(&nxtnode,lFot->Id);
			lFot=lFot->Next;
		}
		
		if(Node[nxtnode->Id].Po==1 && ((Node[nxtnode->Id].Val==D) || (Node[nxtnode->Id].Val==Db)))
			*detected=1;

		DeleteEle(&nxtnode,nxtnode->Id);
	}		
	
}
/*****************/
BACKTRACE backtrace(GATE *Node, int g, int v)
{
	int i, num_inversion,pv;
	LIST *lfin;
	BACKTRACE rv;
	i=g;
	num_inversion=0;
	while(Node[i].Type!=INPT){
		pv =i;
		if((Node[i].Type==NOT)||(Node[i].Type==NAND)||(Node[i].Type==NOR)){//checkimg gate type
			num_inversion++;		
		}
		lfin=Node[i].Fin;
		while(lfin!=NULL){
			if(Node[lfin->Id].Val==DC){//fin value is x
				i=lfin->Id;// store value
				break;
			}				
			lfin=lfin->Next;
		}	
		if(pv == i){
	printf("g=%d,val=%d,type=%d,FinVal1=%d,FinVal2=%d,ObjectiveG=%d **",i,Node[i].Val,Node[i].Type,Node[Node[i].Fin->Id].Val,Node[Node[i].Fin->Next->Id].Val,g);
			PrintList(Node[i].Fin); getchar();
			i=0;
			break;
		}
	}
	if(num_inversion%2!=0){
		v=NOTG[v];
	}
	rv.Gat=i;
	rv.Val=v;
	//printf("gatenumber=%d,gateval=%d",i,v);
	return rv;

}
/*****************/
BACKTRACE getObjective(GATE *Node,  FAULT flt, LIST **Dfrontier)
{
	BACKTRACE rv;
        int d,v=-1,g=-1;
	int targetgate=flt.Fid, saf=flt.ftype;
	LIST *lfin;
	if(Node[targetgate].Val!=D && Node[targetgate].Val!=Db){
		rv.Gat=targetgate;// for actvating fault
		rv.Val=NOTG[saf];
			return rv;
	}
		if(*Dfrontier == NULL){
		 rv.Gat=g;
		 rv.Val=v;		
		 return rv;
		}
		d=(*Dfrontier)->Id;//
		lfin=Node[d].Fin;
		while(lfin!=NULL){
			if(Node[lfin->Id].Val==DC){
				g=lfin->Id;
				break;
			}				
			lfin=lfin->Next;
		}
		v=NonContrl(Node, d);// we need to bring non controlling value in this line
		
		rv.Gat=g;
		rv.Val=v;
		return rv;
}

int NonContrl(GATE *Node, int g)
{
 	if(Node[g].Type==AND || Node[g].Type==NAND) 	return 1;
	else if(Node[g].Type==OR || Node[g].Type==NOR)	return 0;
}
/*****************/
int PodemRecursion(GATE *Node, FAULT flt, LIST **Dfrontier,clock_t start)
{
	BACKTRACE obj, back;
	int detected, faultmask, result;
	
	clock_t end= clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	if(seconds>1) return Timeout;
	
	obj=getObjective(Node, flt, Dfrontier); /// obj will return gate value and demanding value
	if(obj.Gat == -1) return Failure;
	back=backtrace(Node, obj.Gat, obj.Val);//// now fault is mask
	if(back.Gat == 0) return Failure;
	logicSimImply(Node, back.Gat,back.Val,&detected,&faultmask, flt,Dfrontier);
	//PrintList(*Dfrontier);printf("\n");getchar();
	//printf("g=%d, v=%d, pi=%d, u=%d, detected=%d", obj.Gat,obj.Val, back.Gat,back.Val, detected);getchar();
	//PrintGats(Node,23); getchar();
	if(Node[flt.Fid].Val!=flt.ftype){	
		if(detected)	return Success;
		if(faultmask && (*Dfrontier==NULL))   return Failure; 
		result=PodemRecursion(Node,flt,Dfrontier,start);
		if(result==Success) return Success;
		else if(result==Timeout) return Timeout;
	}

	logicSimImply(Node, back.Gat,NOTG[back.Val],&detected,&faultmask, flt,Dfrontier);
	if(Node[flt.Fid].Val!=flt.ftype){
		if(detected)	return Success;
		if(faultmask && (*Dfrontier==NULL))   return Failure;
		result=PodemRecursion(Node,flt,Dfrontier,start);
		if(result==Success) return Success;
		else if(result==Timeout) return Timeout;
	}
	logicSimImply(Node, back.Gat,DC,&detected,&faultmask, flt,Dfrontier);
	return Failure;
	


}

void PrintPatterns(GATE *Node,int Tgat,FILE *result)
{
int i;

for(i=1;i<=Tgat;i++){
  if(Node[i].Type==INPT){
	if(Node[i].Val==D){
		fprintf(result,"1");
		printf("1");
	}		
	else if(Node[i].Val==Db){
		fprintf(result,"0");
		printf("0");
	}
	else if(Node[i].Val==DC){
		fprintf(result,"x");
		printf("X");
	}	
   	else{
		fprintf(result,"%d",Node[i].Val); 
		printf("%d",Node[i].Val);
	}
  }
}
 fprintf(result,"\n");
	printf("\n");
return;
}


