#include "input.h"
/***************************************************************************************************
Command Instructions
***************************************************************************************************/
//To Compile: make
//To Run: ./project c17.isc 
/***************************************************************************************************
 Main Function
***************************************************************************************************/
int main(int argc,char **argv)
{
FILE *Isc,*Pat,*Res;                  //File pointers used for .isc, .pattern, and .res files
int Npi,Npo,Tgat;                     //Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
GATE *Node;                           //Structure to store the ckt given in .isc file 
clock_t Start,End;                    //Clock variables to calculate the Cputime
double Cpu;                           //Total cpu time
int i,j;                              //Temporary variables
int Tpat;
//PATTERN Patterns[Mpt];
BACKTRACE ipt,obj;
FAULT flt;
LIST *Dfrontier=NULL;

/****************PART 1.-Read the .isc file and store the information in Node structure***********/
Npi=Npo=Tgat=0;                                //Intialize values of all variables
Isc=fopen(argv[1],"r");                        //File pointer to open .isc file 
Node=(GATE *) malloc(Mnod * sizeof(GATE));     //Dynamic memory allocation for Node structure
Tgat=ReadIsc(Isc,Node);                        //Read .isc file and return index of last node
fclose(Isc);                                   //Close file pointer for .isc file
PrintGats(Node,Tgat);                          //Print the information of each active gate in Node structure after reading .isc file
CountPri(Node,Tgat,&Npi,&Npo);                 //Count the No of Pis and Pos
printf("\n\nNpi: %d Npo: %d\n",Npi,Npo); 

//fprintf(result,"\n");
printf("\n");

//Print the no of primary inputs and outputs
/***************************************************************************************************/
//Pat=fopen(argv[2],"r");                        //File pointer to open .isc file 
//Tpat=ReadPat(Pat,Patterns);                     //Read .isc file and return index of last node
//fclose(Pat);
//Res=fopen(argv[3],"w");
//LogicSim(Node, Patterns, Tgat, Tpat, Res);
//fclose(Res);
/***************************************************************************************************/
FILE *result;
int s=0,f=0,t=0,state;
clock_t start; 
result=fopen(argv[2],"w");
for(j=0;j<=Tgat;j++){
	if(Node[j].Type!=0){
		flt.Fid=j;
		for(i=0;i<2;i++){
			flt.ftype=i;
			fprintf(result,"Fault:%d SA:%d\n",flt.Fid,flt.ftype);
			printf("Fault:%d SA:%d\n",flt.Fid,flt.ftype);
			start=clock();
			resetckt(Node,Tgat);
			FreeList(&Dfrontier);//an address passes tjrough the pointer
			state=PodemRecursion(Node,flt,&Dfrontier,start);
		 	if(state==Success){
				s++;
				PrintPatterns(Node,Tgat,result);
			}
			else if(state==Failure)
				f++;
			else
				t++;
			
		}
	}
}

//fprintf(result,"\n");
printf("\n");

printf("Total SAF: %d\n", s+f+t);
fprintf(result,"Total SAF: %d\n", s+f+t);
printf("SAF Success: %d\n",s);
fprintf(result,"SAF Success: %d\n",s);
printf("SAF Failure: %d\n",f);
fprintf(result,"SAF Failure: %d\n",f);
printf("SAF Timeout: %d\n",t);
fprintf(result,"SAF Timeout: %d\n",t);
printf("SAF Coverage: %.2f%%\n",(double)s*100/(s+f+t));
fprintf(result,"SAF Coverage: %.2f%%\n",(double)s*100/(s+f+t));
fclose(result); 
//obj = getObjective(Node,flt,&Dfrontier);
//ipt = backtrace(Node,obj.Gat,obj.Val);
//logicSimImply(Node, ipt.Gat, ipt.Val,flt,&Dfrontier);

/****************************************************************************************************************************/
ClearGat(Node,Tgat);  free(Node);                                      //Clear memeory for all members of Node
return 0;
}//end of main
/****************************************************************************************************************************/
