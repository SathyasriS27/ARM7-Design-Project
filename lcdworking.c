//PINSEL0 controls PORT0 pins P0.0 to P0.15, PINSEL1 controls PORT0 pins P0.16 to P0.31 and PINSEL2 controls PORT1 pins P1.16 to P1.31.
//IOxSET - To set an output configured pin
//IOxCLR - To clear an output configured pin
//IOxPIN - To get logic value on a I/O pin
//IOxDIR - To select input /output function (by placing 0/1) for an I/O pin
//ldc is 16X2 i.e. can display 16 characters 2 lines at a time
#include<lpc21xx.h>
#include<string.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
void delay(void);
void lcd(char,int);
void lcd_display(char s[]);
void waitforkeypress(void);
void enterchar(char s[]);
char keypad(void);
char k; //pressed keys are assigned to this variable
struct complex
{
double real, img;
};
int main(void)
{
double n1,n2,n3;
double out_real,out_img,freal,fimg,ireal,iimg;
struct complex s1,s2;
char ch[]="1)a+ib 2)c+id";
char ch2[]="Enter a:";
char ch3[]="Enter b:";
char ch4[]="Enter c:";
char ch5[]="Enter d:";
char ch6[]="Output:";
char err[]="ERROR: 0/0 div!";
char test='T';
char a[6];
char b[6];
char c[6];
char d[6];
char rreal[16];
char rimg[6];
PINSEL0=PINSEL2=0; //configure pins as GPIO
IODIR0=0x000003ff; //pins P0.0 to 9
IODIR1=0x00f80000; //pins P1.19 to 23, P1.16 to 18 is set as 0 hence I/P pins
lcd(0x38,0); //0x38 is lcd command to initialise lcd in 8 bit mode
lcd(0x0f,0); //0x0f is command for - display on,cursor blinking
while(1)
{ a[0]='\0'; //to clear/empty the strings
b[0]='\0';
c[0]='\0';
d[0]='\0';
rreal[0]='\0';
rimg[0]='\0';
n1=0; //resetting values for current iteration
n2=0;
n3=0;
out_real=0;
out_img=0;
lcd_display(ch); //display string ch on lcd
waitforkeypress(); //wait until key is pressed
lcd(0x01,0); //LCD cmd to clear screen
lcd_display(ch2); //display string ch2 on lcd
lcd(0x0c0,0); //lcd cmd to force cursor to 2nd line
enterchar(a); //enter characters for string a
lcd(0x01,0);
lcd_display(ch3); //display string ch3 on lcd
lcd(0x0c0,0);
enterchar(b); //enter characters for string b
lcd(0x01,0);
lcd_display(ch4); //display string ch4 on lcd
lcd(0x0c0,0);
enterchar(c); //enter characters for string c
lcd(0x01,0);
lcd_display(ch5); //display string ch5 on lcd
lcd(0x0c0,0);
enterchar(d); //enter characters for string d
lcd(0x01,0);
//code to check if string arrays are inititalised properly by printing input
lcd_display(a); //display string a on lcd
lcd(0x0c0,0);
lcd_display(b); //display string b on lcd
waitforkeypress();
lcd(0x01,0);
lcd_display(c); //display string c on lcd
lcd(0x0c0,0);
10
lcd_display(d); //display string d on lcd
waitforkeypress();
lcd(0x01,0);
//code to divide 2 complex numbers-
s1.real=strtod(a,NULL);
//convert string a to double s1.real using strtod , 2nd arg, endptr=NULL
s1.img=strtod(b,NULL);
s2.real=strtod(c,NULL);
s2.img=strtod(d,NULL);
if (s2.real==0 && s2.img==0)
lcd_display(err);//show error msg if c and d both are 0
else
{
n1 = s1.real*s2.real + s1.img*s2.img;
n2 = s1.img*s2.real - s1.real*s2.img;
n3 = s2.real*s2.real + s2.img*s2.img;
out_real=n1/n3;
out_img=n2/n3;
freal=modf(out_real,&ireal);
//splits the integer part and fractional part of out_real into variables ireal and freal respectively
fimg=modf(out_img,&iimg);
//splits the integer part and fractional part of out_img into variables iimg and fimg respectively
if(out_real<0 && out_img<0)//if both real and img terms are negative
{
sprintf(rreal,"-%d.%02u\0",(int)ireal,(int)fabs(freal*100));
sprintf(rimg,"-%d.%02u\0",(int)iimg,(int)fabs(fimg*100));
}
else if(out_real<0)//if real term is negative
{
sprintf(rreal,"-%d.%02u\0",(int)ireal,(int)fabs(freal*100));
sprintf(rimg,"%d.%02u\0",(int)iimg,(int)fabs(fimg*100));
}
else if(out_img<0)//if img term is negative
{
sprintf(rreal,"%d.%02u\0",(int)ireal,(int)fabs(freal*100));
sprintf(rimg,"-%d.%02u\0",(int)iimg,(int)fabs(fimg*100));
}
Else //if both real and img terms are positive
{
sprintf(rreal,"%d.%02u\0",(int)ireal,(int)fabs(freal*100));
sprintf(rimg,"%d.%02u\0",(int)iimg,(int)fabs(fimg*100));
}
lcd(test,1); //print test character 'T' to check if code has executed uptil here
  11
waitforkeypress();
lcd(0x01,0);
strcat(rreal,"+i(");
strcat(rreal,rimg);
strcat(rreal,")\0");
lcd_display(ch6); //display string ch6 on lcd
lcd(0x0c0,0);
lcd_display(rreal); //display string rreal on lcd (final result)
}
waitforkeypress();
lcd(0x01,0);
}
}
void lcd(char a,int b) //LCD Subroutine
{
IOSET0=a<<0;
IOSET0=b<<8;
//P0.8 is connected to Register select RS, when set to 1, displays data output, when set to 0, treats input as command
IOSET0=1<<9; //P0.9 is connected to Lcd Enable
delay();
IOCLR0=1<<9;
IOCLR0=b<<8;
IOCLR0=a<<0;
}
void lcd_display(char s[]) //to display string on LCD
{
int i=0;
for(i=0;s[i]!='\0';i++)
lcd(s[i],1);
}
void enterchar(char s[]) //to append characters entered through keypad into string
{
int i=0;
while(1)
{
k=keypad(); //Obtaining values from keypad
if(k=='E')
{
s[i]='\0';
break;
}
s[i]=k;
i++;
lcd(k,1);
}
}
char keypad(void) //Keypad Scan
{
while(1)
{
IOCLR1|=(1<<19); //Making row1 LOW (P1.19)
IOSET1|=(1<<20)|(1<<21)|(1<<22)|(1<<23); //Making rest of the rows '1'
if(!(IOPIN1&(1<<16))) //Scan for key press (P1.16 is column 1)
// i guess if button is pressed then corresponding bit will be 0
{
while(!(IOPIN1&(1<<16)));
return '1'; //Returning value to display
}
if(!(IOPIN1&(1<<17)))
{
while(!(IOPIN1&(1<<17)));
return '2';
}
if(!(IOPIN1&(1<<18)))
{
while(!(IOPIN1&(1<<18)));
return '3';
}
IOCLR1|=(1<<20);
IOSET1|=(1<<21)|(1<<22)|(1<<19)|(1<<23);
if(!(IOPIN1&(1<<16)))
{
while(!(IOPIN1&(1<<16)));
return '4';
}
if(!(IOPIN1&(1<<17)))
{
while(!(IOPIN1&(1<<17)));
return '5';
}
if(!(IOPIN1&(1<<18)))
{
while(!(IOPIN1&(1<<18)));
return '6';
}
IOCLR1|=(1<<21);
IOSET1|=(1<<22)|(1<<20)|(1<<19)|(1<<23);
if(!(IOPIN1&(1<<16)))
{
while(!(IOPIN1&(1<<16)));
return '7';
}
if(!(IOPIN1&(1<<17)))
{
while(!(IOPIN1&(1<<17)));
return '8';
}
if(!(IOPIN1&(1<<18)))
{
while(!(IOPIN1&(1<<18)));
return '9';
}
IOCLR1|=(1<<22);
IOSET1|=(1<<19)|(1<<20)|(1<<21)|(1<<23);
if(!(IOPIN1&(1<<16)))
{
while(!(IOPIN1&(1<<16)));
return '0';
}
if(!(IOPIN1&(1<<17)))
{
while(!(IOPIN1&(1<<17)));
return '-';
}
if(!(IOPIN1&(1<<18)))
{
while(!(IOPIN1&(1<<18)));
return 'E';
}
}
}
void waitforkeypress(void)
{
while(1) //wait for keypress to clear screen and display next line
{
k=keypad(); //Obtain any value from keypad
break;
}
}
void delay(void) //Delay loop
{
unsigned int i;
for(i=0;i<=20000;i++);
}
