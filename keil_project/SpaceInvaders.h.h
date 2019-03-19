// dac.h
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 8/25/2014 
// Last Modified: 10/5/2014 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

extern struct paddle{
	int leftx;
	int boty;
}pad;

extern struct wball{
	int leftx;
	int boty;
	int velx;
	int vely;
}wbal;

extern struct bball{
	int leftx;
	int boty;
}bbal;



extern struct nxtball{
	int leftx;
	int boty;
	int velx;
	int vely;
}nxtbal;
	
extern struct brick{
	int leftx;
	int boty;
	int health;
}one,two,three,four,five;

extern struct laser{
	int botlft;
	int toplft;
	int vely;
}lazr;
