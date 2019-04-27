#define INCL_WIN
#define MYAPP "MyApp"
#include <os2.h>
#include <stdio.h>
#include "over2.h"

static MRESULT EXPENTRY overloadwindow (HWND, ULONG, MPARAM, MPARAM);
void OverloadHuman( short Z, short S, HWND QWE);
void OverloadComputer( short Z, short S, HWND QWE);
void Computer(void);
 
HAB     hab1;
HWND    hwndClient1,hwndFrame1,hwndButton, test;
POINTL ptl;
HBITMAP hbitmap;
SIZEL sizl;
HPS hps;
HDC hdc;
short s,z; /* field coordinates s: column z: row*/
short array_count[30][30]; /* 0:Empty 1: 1 Stone 2: 2 Stones 3: 3 Stones */
short array_who[30][30];  /* 0:Empty 1:Human 2:Computer */
BOOL human=TRUE; /* TRUE: Humans turn; FALSE: Computers turn */
short Q[30][30]; /* Quality */
int x,y; /* Mousecoordinates */
int h=0; /* Count of players fields */
int c=0; /* Count of computers fields */
short pix=32;
short x_field=9; 
short y_field=9;
int idbmp;
BOOL start=TRUE;
BOOL restart=FALSE;
SWP winpos;
 
 
void main (void)
{
	HMQ     hmq1;
	QMSG    qmsg1;
	ULONG   flCreate1,wsCreate1;
	hab1 = WinInitialize (0);
	if (!hab1)
	return;

	hmq1 = WinCreateMsgQueue (hab1, 0);
	WinRegisterClass(hab1, MYAPP, (PFNWP) overloadwindow, CS_SIZEREDRAW,	0);
	flCreate1 = (FCF_TITLEBAR | FCF_SYSMENU | FCF_DLGBORDER | FCF_MINBUTTON | FCF_MENU |
				 FCF_ACCELTABLE | FCF_ICON | FCF_TASKLIST); 
	wsCreate1 = ( WS_VISIBLE );

	hwndFrame1 = WinCreateStdWindow(HWND_DESKTOP, wsCreate1, &flCreate1, MYAPP, "Overload/2", 0L, NULLHANDLE, ID_PM, &hwndClient1);
	WinQueryTaskSizePos(hab1, 0, &winpos);
	WinSetWindowPos( hwndFrame1, HWND_TOP, winpos.x, winpos.y, (x_field*(pix-1)+9), (y_field*(pix-1)+54), SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE ); 

 
		while (WinGetMsg (hab1,&qmsg1,(HWND)NULL,0,0))
		WinDispatchMsg (hab1,&qmsg1);

	WinDestroyWindow(hwndButton); 
	WinDestroyWindow(hwndFrame1);
	WinDestroyMsgQueue(hmq1);
	WinTerminate(hab1);
}
 
MRESULT EXPENTRY overloadwindow (HWND test, ULONG msg, MPARAM mp1, MPARAM mp2)
{
switch (msg)
{
case WM_PAINT:
	hps=WinBeginPaint(test,NULLHANDLE,NULL);
	hdc=WinOpenWindowDC(test);
	sizl.cx=sizl.cy=0;
	ptl.x=ptl.y=0;
	hps=GpiCreatePS(hab1,hdc,&sizl,PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC);

	if (restart==TRUE)
	{
	for(s=1;s<=x_field;s++)
		{
		for(z=1;z<=y_field;z++)
			{
			array_count[z][s]=0;
			array_who[z][s]=0;
			}
		}
	restart=FALSE;
	human=TRUE;
		WinSetWindowPos( hwndFrame1,
                               HWND_TOP,
                               winpos.x, winpos.y, (x_field*(pix-1)+9), (y_field*(pix-1)+54),
                               SWP_SIZE | SWP_SHOW | SWP_ACTIVATE ); 

	} //end if (restart==TRUE)
	
	if (start==TRUE){
	for(s=1;s<=x_field;s++)
		{
		for(z=1;z<=y_field;z++)
			{
			if (array_who[z][s]==0) array_count[z][s]=0;
			if (array_who[z][s]==2){idbmp=204+array_count[z][s];} else {idbmp=200+array_count[z][s];}
			GpiDeleteBitmap(hbitmap);
			hbitmap=GpiLoadBitmap(hps,NULLHANDLE,idbmp,pix,pix);
			WinDrawBitmap(hps,hbitmap,NULL,&ptl,CLR_NEUTRAL,CLR_BACKGROUND,DBM_NORMAL);
			ptl.y=ptl.y+(pix-1);
			}
		ptl.x=ptl.x+(pix-1);ptl.y=0;
		}
         } //end if (start==TRUE)

	else if(z<=y_field && s<=x_field)
		{
		if (array_who[z][s]==2){idbmp=204+array_count[z][s];}else {idbmp=200+array_count[z][s];}
		ptl.x=(s-1)*(pix-1);ptl.y=(z-1)*(pix-1);
		GpiDeleteBitmap(hbitmap);
		hbitmap=GpiLoadBitmap(hps,NULLHANDLE,idbmp,pix,pix);
		WinDrawBitmap(hps,hbitmap,NULL,&ptl,CLR_NEUTRAL,CLR_BACKGROUND,DBM_NORMAL);
		}

	start=TRUE;
	WinEndPaint (hps);
	return 0;

break;
	
case WM_DESTROY:
	GpiDeleteBitmap(hbitmap);
	GpiDestroyPS(hps);	
break;

case WM_ERASEBACKGROUND:
	return (MRESULT) TRUE;
break;
	
case WM_CLOSE:
	if (WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,
		"Are you sure you want to quit?","Overload/2",
		0,MB_YESNO | MB_QUERY) == MBID_YES)
	WinPostMsg(test,WM_QUIT,0L,0L);

break;

case WM_BUTTON1CLICK:
		start=TRUE;
		x=SHORT1FROMMP(mp1);
		y=SHORT2FROMMP(mp1);
		z=y/(pix-1)+1;s=x/(pix-1)+1;


		if(z<=y_field && z>=1 && s<=x_field && s>=1)
			{
			if (array_who[z][s]==1 || array_who[z][s]==0)
				{
				array_who[z][s]=1;
				array_count[z][s]++;
				if (array_count[z][s]>3) OverloadHuman(z,s,test);
				human=FALSE;
				}
			else   DosBeep( 440,250 );
			}
		
 		WinPostMsg(test,WM_PAINT,0L,0L);
		c=0;
		h=0;
		for(s=1;s<=x_field;s++) /* Count the amount of computers fields */
			{
			for(z=1;z<=y_field;z++)
				{
				if (array_who[z][s]==2) {c++;}
				if (array_who[z][s]==1) {h++;}
				}
			}
		if (c==0 && h>1)
		{
		WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,"You win!!!","Overload/2",0,MB_OK | MB_NOICON);
		restart=TRUE;	start=TRUE;WinPostMsg(test,WM_PAINT,0L,0L);
		}
	
		
		if (human==FALSE)
		{
		Computer(); 
		if(z<=y_field && z>=1 && s<=x_field && s>=1) 
			{
			if (array_who[z][s]==2 || array_who[z][s]==0)
				{
				array_who[z][s]=2;
				array_count[z][s]++;
				if (array_count[z][s]>3) OverloadComputer(z,s,test);
				human=TRUE;
				}
			else   DosBeep( 440,250 ) ;
			}
		}

		WinPostMsg(test,WM_PAINT,0L,0L);

 		h=0;
		c=0;
		for(s=1;s<=x_field;s++) /* Count the amount of players fields */
			{
			for(z=1;z<=y_field;z++)
				{
				if (array_who[z][s]==1) {h++;}
				if (array_who[z][s]==2) {c++;}
				}
			}
		if (h==0)
		{
		for(s=1;s<=x_field;s++)
			{
			for(z=1;z<=y_field;z++)
				{
				array_who[z][s]=2;
				}
			}
		WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,"You lost!!!","Overload/2",0,MB_OK | MB_NOICON);
		restart=TRUE;	start=TRUE;WinPostMsg(test,WM_PAINT,0L,0L);
		}
				
break;
	
case WM_COMMAND:
	switch (SHORT1FROMMP (mp1))
	{
	case IDM_EXIT:
		{WinSendMsg (test, WM_CLOSE, NULL, NULL);return (MRESULT)0;}
		break;

	case IDM_INFO:
		WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,
		"Overload/2 v0.9 beta, Copyright (C) 2008 Johannes Urscher, Overload/2 comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome to distribute it under certain conditions; see file 'copying' for more details.
Report bugs to: Johannes.Urscher@gmx.de",
		"Overload/2", 0, MB_OK | MB_NOICON);
		WinPostMsg(test,WM_PAINT,0L,0L);
		break;

	case IDM_NEW:
		{restart=TRUE;	start=TRUE;WinPostMsg(test,WM_PAINT,0L,0L);}
		break;
	}
break;
	  
default:
		return WinDefWindowProc (test, msg, mp1, mp2);
}
return FALSE;
}

void OverloadHuman( short Z, short S, HWND QWE )
{
if(Z>=0 && S>=0 && Z<=(y_field+1) && S<=(x_field+1)){

                        array_who[Z+1][S]=1;
array_who[Z][S-1]=1;                         array_who[Z][S+1]=1;  
                        array_who[Z-1][S]=1;

if((Z+1)>=1 && (Z+1)<=y_field && S>=1 && S<=x_field) array_count[Z+1][S]++; 
if(Z>=1 && Z<=y_field && (S-1)>=1 && (S-1)<=x_field) array_count[Z][S-1]++;  if(Z>=1 && Z<=y_field && (S+1)>=1 && (S+1)<=x_field) array_count[Z][S+1]++;  
if((Z-1)>=1 && (Z-1)<=y_field && S>=1 && S<=x_field) array_count[Z-1][S]++; 

array_count[Z][S]=0;
array_who[Z][S]=0;


WinPostMsg(QWE,WM_PAINT,0L,0L);

				if (array_count[Z+1][S]>3)OverloadHuman(Z+1,S,QWE) ;
if (array_count[Z][S-1]>3) OverloadHuman(Z,S-1,QWE); if (array_count[Z][S+1]>3)OverloadHuman(Z,S+1,QWE) ;  
				if (array_count[Z-1][S]>3)OverloadHuman(Z-1,S,QWE) ;
start=TRUE;
}}

void OverloadComputer( short Z, short S, HWND QWE)
{
if(Z>=0 && S>=0 && Z<=(y_field+1) && S<=(x_field+1)){


                        array_who[Z+1][S]=2;
array_who[Z][S-1]=2;                         array_who[Z][S+1]=2;  
                        array_who[Z-1][S]=2;

if((Z+1)>=1 && (Z+1)<=y_field && S>=1 && S<=x_field) array_count[Z+1][S]++; 
if(Z>=1 && Z<=y_field && (S-1)>=1 && (S-1)<=x_field) array_count[Z][S-1]++; if(Z>=1 && Z<=y_field && (S+1)>=1 && (S+1)<=x_field) array_count[Z][S+1]++;  
if((Z-1)>=1 && (Z-1)<=y_field && S>=1 && S<=x_field) array_count[Z-1][S]++; 

array_count[Z][S]=0;
array_who[Z][S]=0;

WinPostMsg(QWE,WM_PAINT,0L,0L);
				if (array_count[Z+1][S]>3)OverloadComputer(Z+1,S,QWE) ;
if (array_count[Z][S-1]>3) OverloadComputer(Z,S-1,QWE); if (array_count[Z][S+1]>3)OverloadComputer(Z,S+1,QWE) ;  
				if (array_count[Z-1][S]>3)OverloadComputer(Z-1,S,QWE) ;
start=TRUE;
}}

void Computer(void)
{
int p,q,w,h1;
p=0;q=0;w=0;h1=0;
for(s=1;s<=x_field;s++)
	{
	for(z=1;z<=y_field;z++)
		{
		Q[z][s]=150;
		}
	}

for(s=1;s<=x_field;s++)
	{
	for(z=1;z<=y_field;z++)
		{
		if(array_who[z][s]==1) Q[z][s]+=50;                               // Players fields
		// if(array_count[z][s]==4 && array_who[z][s]==2) Q[z][s]-=2;
		if(array_who[z][s]==2) Q[z][s]++;
		if((s==1 || s==x_field) || (z==1 || z==y_field)) Q[z][s]++;        // Edges
		if(s==z && (z==1 || z==y_field)) Q[z][s]++;                       // Corners
		if((s==1 && z==y_field) || (z==1 && s==x_field)) Q[z][s]++;     // Corners

		if(array_who[z][s]==2 && array_count[z][s]==4)            
			{
			h1=0;
			if(array_who[z+1][s]==1) h1=h1+5;
			if(array_who[z][s+1]==1) h1=h1+5;
			if(array_who[z-1][s]==1) h1=h1+5;
			if(array_who[z][s-1]==1) h1=h1+5;
			Q[z][s]-=h1;
			}
		if(array_who[z][s]==2)
			{
			h1=0;
			if(array_who[z+1][s]==1 && array_count[z+1][s]<=array_count[z][s]) h1=h1+6;
			if(array_who[z][s+1]==1 && array_count[z][s+1]<=array_count[z][s]) h1=h1+6;
			if(array_who[z-1][s]==1 && array_count[z-1][s]<=array_count[z][s]) h1=h1+6;
			if(array_who[z][s-1]==1 && array_count[z][s-1]<=array_count[z][s]) h1=h1+6;
			Q[z][s]-=h1;
			}
		
		
		if(( (array_count[z][s-1]==4 && array_who[z][s-1]==1) || 
		    (array_count[z+1][s]==4 && array_who[z+1][s]==1) || 
		    (array_count[z][s+1]==4 && array_who[z][s+1]==1) || 
		    (array_count[z-1][s]==4 && array_who[z-1][s]==1)     )  
		   && array_count[z][s]<4) Q[z][s]+=5;
		if( (array_count[z][s-1]>array_count[z][s] && array_who[z][s-1]==1) || 
		    (array_count[z+1][s]>array_count[z][s] && array_who[z+1][s]==1) || 
		    (array_count[z][s+1]>array_count[z][s] && array_who[z][s+1]==1) || 
		    (array_count[z-1][s]>array_count[z][s] && array_who[z-1][s]==1)     ) Q[z][s]+=5;
		if( (array_count[z][s-1]>array_count[z][s] && array_who[z][s-1]==1) && 
		    (array_count[z+1][s]>array_count[z][s] && array_who[z+1][s]==1) && 
		    (array_count[z][s+1]>array_count[z][s] && array_who[z][s+1]==1) && 
		    (array_count[z-1][s]>array_count[z][s] && array_who[z-1][s]==1)     ) Q[z][s]+=15;
		if( (array_count[z+1][s-1]==array_count[z][s] && array_who[z+1][s-1]==1) || 
		    (array_count[z+1][s+1]==array_count[z][s] && array_who[z+1][s+1]==1) || 
		    (array_count[z-1][s+1]==array_count[z][s] && array_who[z-1][s+1]==1) || 
		    (array_count[z-1][s-1]==array_count[z][s] && array_who[z-1][s-1]==1)     )   Q[z][s]+=3;


		if( (array_count[z][s-1]==array_count[z][s]==4 && array_who[z][s-1]==1) || 
		    (array_count[z+1][s]==array_count[z][s]==4 && array_who[z+1][s]==1) || 
		    (array_count[z][s+1]==array_count[z][s]==4 && array_who[z][s+1]==1) || 
		    (array_count[z-1][s]==array_count[z][s]==4 && array_who[z-1][s]==1)     )  Q[z][s]-=10;
		if( (array_count[z][s-1]==array_count[z][s]==4 && array_who[z][s-1]==1) && 
		    (array_count[z+1][s]==array_count[z][s]==4 && array_who[z+1][s]==1) && 
		    (array_count[z][s+1]==array_count[z][s]==4 && array_who[z][s+1]==1) && 
		    (array_count[z-1][s]==array_count[z][s]==4 && array_who[z-1][s]==1)     )  Q[z][s]-=30;
/*
		if( (array_count[z][s-1]==array_count[z][s]==4 && array_who[z][s-1]==2) || 
		    (array_count[z+1][s]==array_count[z][s]==4 && array_who[z+1][s]==2) || 
		    (array_count[z][s+1]==array_count[z][s]==4 && array_who[z][s+1]==2) || 
		    (array_count[z-1][s]==array_count[z][s]==4 && array_who[z-1][s]==2)     )  Q[z][s]-=1;
		if( (array_count[z][s-1]==array_count[z][s]==4 && array_who[z][s-1]==2) && 
		    (array_count[z+1][s]==array_count[z][s]==4 && array_who[z+1][s]==2) && 
		    (array_count[z][s+1]==array_count[z][s]==4 && array_who[z][s+1]==2) && 
		    (array_count[z-1][s]==array_count[z][s]==4 && array_who[z-1][s]==2)     )  Q[z][s]-=2;
*/
		if( (array_who[z][s-1]==1 && array_count[z][s]==4) && 
		    (array_who[z+1][s]==1 && array_count[z][s]==4) && 
		    (array_who[z][s+1]==1 && array_count[z][s]==4) && 
		    (array_who[z-1][s]==1 && array_count[z][s]==4)     )  Q[z][s]-=10;
		if( (array_who[z][s-1]==1 && array_count[z][s]==4) || 
		    (array_who[z+1][s]==1 && array_count[z][s]==4) || 
		    (array_who[z][s+1]==1 && array_count[z][s]==4) || 
		    (array_who[z-1][s]==1 && array_count[z][s]==4)     )  Q[z][s]-=5;
		if(( (array_count[z][s-1]==array_count[z][s] && array_who[z][s-1]==1) || 
		    (array_count[z+1][s]==array_count[z][s] && array_who[z+1][s]==1) || 
		    (array_count[z][s+1]==array_count[z][s] && array_who[z][s+1]==1) || 
		    (array_count[z-1][s]==array_count[z][s] && array_who[z-1][s]==1)     )  
		  && array_count[z][s]<4) Q[z][s]-=4;
		if(( (array_count[z][s-1]==array_count[z][s] && array_who[z][s-1]==2) || 
		    (array_count[z+1][s]==array_count[z][s] && array_who[z+1][s]==2) || 
		    (array_count[z][s+1]==array_count[z][s] && array_who[z][s+1]==2) || 
		    (array_count[z-1][s]==array_count[z][s] && array_who[z-1][s]==2)     )  
		  && array_count[z][s]<4) Q[z][s]-=2;
		
		}
	}
while(q==0)
	{
	for(s=1;s<=x_field;s++)
		{
		for(z=1;z<=y_field;z++)	
			{
			if (Q[z][s]==w) {p=s; q=z;}
			}
		}
	w++;
	}
z=q;
s=p;
} 
