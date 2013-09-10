#include <string>
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cmath>
#define SECTOR_UB 5000
#define NUM_REQUEST 5
#define INIT_TRACK 5
#define CACHE_SIZE 3
using namespace std;

int track,cachehit=0,totalseek=0;


void f()
{
    std::string line;
    while(std::getline(std::cin, line))  //input from the file in.txt
    {
        std::cout << line << "\n";   //output to the file out.txt
    }
}

bool checkCache(int sector_num)
{
	ifstream myfile;
  	myfile.open("/virtual6/CacheList");
	cout<<"Sectors in Cache : ";
	while ( myfile.good() )
	{
		string line;
	  	getline (myfile,line);
		int value = atoi(line.c_str());
		if(value!=0)
			cout<<value<<" ";
		if(value==sector_num)
		{
			myfile.close();
			return true;
		}
	}
	cout<<"\n";
	myfile.close();
	return false;
}


void readfromCache(int sector_num)
{
	string c="/virtual6/Disk-Cache/Sector";

	stringstream sstm;
	sstm << c << sector_num;
	string result = sstm.str();
	
	ifstream myfile;
  	myfile.open(result.c_str());
	cout<<"\nReading Sector From Cache.....\n";
	cout<<result<<"\n";
		
	while ( myfile.good() )
	{
		string line;
	  	getline (myfile,line);
	 	cout << line << endl;		
	}
    	myfile.close();
}

void collision(int sector_num)
{
	fstream filestr;
	filestr.open ("/virtual6/CacheList", fstream::in | fstream::out | fstream::app);
	filestr<<to_string(sector_num)<<endl;
	filestr.close();
		 
	ifstream myfile;
	ofstream newfile;
  	myfile.open("/virtual6/CacheList");
	newfile.open("/virtual6/listtemp");
	
	int i=0;
	string line,del;
	if ( myfile.good() )
	{
		getline (myfile,del);
		i++;
	}
	while ( myfile.good())
	{
	  	getline (myfile,line);
		if(atoi(line.c_str())!=0)
		{
			newfile<<line<<endl;	
			i++;	
		}
	}
	myfile.close();
	newfile.close();	
		
	if(i>CACHE_SIZE)
	{
		string addr="/virtual6/Disk-Cache/Sector";
		stringstream m;
		m << addr << del;
		string remf=m.str();
		remove(remf.c_str());
		remove("/virtual6/CacheList");
		rename("/virtual6/listtemp","/virtual6/CacheList");
	}
	else
	{
		 remove("/virtual6/listtemp");		
	}
}

void gotosector(int surface_num, int sector_num)
{
	string add="/virtual";
	string b="/Track";
	string c="/Sector";

	stringstream sstm;
	sstm << add << 	surface_num+1 << b << track << c << sector_num;
	string result = sstm.str();	
	ifstream myfile;
	myfile.open(result.c_str());


	string addr="/virtual6/Disk-Cache/Sector";
	stringstream m;
	m << addr << sector_num;
	string newf=m.str();
	ofstream buffile;
  	
	
	if(surface_num==sector_num/1000)
	{		
		collision(sector_num);
		buffile.open(newf.c_str());
		cout<<"Reading Sector.....\n";
		cout<<result<<"\n";
		if ( myfile.good() )
		{
			string line;
		  	getline (myfile,line);
		  	cout << line << endl;
		  	buffile << line <<endl;
		}
		buffile.close();
		/*ifstream bffile;	
		bffile.open(newf.c_str());
		cout<<"reading buffile\n"<<newf;
		while ( bffile.good() )
		{
			string line;
		  	getline (bffile,line);
		  	cout << line << endl;
		}
		bffile.close();
*/
	}
    	myfile.close();

}


/* Readme

@struct sector_track : associates a track number with a sector.

@function main : the testing function

@function func : sorts an array using sstf

@function func1 : sorts an sector_track array using track no.

@param a[] input array
@param b[] output array using sstf
@param len length of array a[]
@param head the initial position of head on track

@function func2 : algorithm to sort using sstf

@function func3 : algorithm to sort using cLook/cScan

*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct sector_track
{
    int sector;
    int track;
};

int compare (const void * a, const void * b)
{
    //cout<<(*(sector_track*)a).sector<<"in compare, sector\n";
  return ( (*(sector_track*)a).track - (*(sector_track*)b).track );
}

void func3(sector_track a[],sector_track b[],int len,int head)
{
	cout<< "Using CLOOK\n";
    qsort (a, len, sizeof(sector_track), compare);
	
    int j = 0;
    while(a[j].track<head && j<len)
    {
        j++;
    }
    int i = j;
    while(j<len)
    {
        b[j-i] = a[j];
        j++;
    }
    for(int k = len-i;k<len;k++)
        b[k] = a[k-len+i];

}

void func2(sector_track a[],sector_track b[],int len,int head)
{
	cout << "Using SSTF\n";
    qsort (a, len, sizeof(sector_track), compare);

    /*
	for(int k=0;k<len;k++)
    {
        cout<<a[k].sector<<"sector\n";
    }*/


    //int i=search(start);
    int i=0;
    int abso=abs(head-a[0].track);
    for(int j=0;j<len;j++)
    {


        if(abs(head-a[j].track)<abso)
        {
            abso=abs(head-a[j].track);
            i=j;
        }
        else
        {
            abso=abso;
        }
    }


    int left=i-1,right=i+1;
    int a_len=len;
    //int b[len];
    int i2=0;
    b[i2]=a[i];


    while(1)
    {


        if(right==a_len)
        {
            for(int i=left;i>-1;i--)
            {
                i2++;
                b[i2]=a[i];

            }
            break;
        }

        else if(left==-1)
        {
            for(int i=right;i<a_len;i++)
            {
                //cout<<"in elseif : i="<<i<<"\n";
                i2++;
                b[i2]=a[i];
                //cout<<"b[i2]"<<b[i2]<<"\n";

            }
            break;
        }
        else {

        if(a[i].track-a[left].track<a[right].track-a[i].track)
        {
            i=left;
            left--;
            //cout<<"in if : i="<<i<<"\n";
        }
        else
        {
            i=right;
            right++;
            //cout<<"in else : i="<<i<<"right ="<<right<<"\n";
        }
        i2++;
        b[i2]=a[i];
        }

    }


}

void func1(int a[], int b[], int len, int head)
{
    sector_track associative_array[len];
    for(int i=0;i<len;i++)
    {
        associative_array[i].sector=a[i];
        associative_array[i].track=((a[i]%1000)/100);

    }
    sector_track associative_array2[len];
    func2(associative_array,associative_array2,len,head );

    for(int k=0;k<len;k++)
    {
        b[k]=associative_array2[k].sector;
        //cout<<associative_array2[k].sector<<"assoc\n";

    }
}

void genrequests(int *request){
	srand(time(NULL));
	for(int i=0; i<NUM_REQUEST; i++){
		request[i] = rand()%SECTOR_UB;
	}
}


int main()
{
	int a[NUM_REQUEST];
	int b[NUM_REQUEST];
	
      	cout << "Number of requests:"<<NUM_REQUEST<<"\nInitial Track:"<<INIT_TRACK<<"\nDisk Buffer Size:"<<CACHE_SIZE<<"\n";
	genrequests(a);
	cout << "Generated requests :\n";
	for(int i=0;i<NUM_REQUEST;i++)
	{
		cout << a[i] <<"  ";
	}
	cout << "\n\n";	
	//genrequests1(a);
	func1(a,b,NUM_REQUEST,INIT_TRACK);
	cout << "Order of read :\n";
	for(int i=0;i<NUM_REQUEST;i++)
	{
		cout << b[i] <<"  ";
	}
	cout << "\n\n";

	int prevtrack=INIT_TRACK;
	for(int i=0;i<NUM_REQUEST;i++)
	{
		int sector_num=b[i];
		track = (sector_num%1000)/100;

		if(checkCache(sector_num))
		{
			readfromCache(sector_num);
			cout<< "Seektime : 0" <<"\n\n\n";
			cachehit++;					
		}
		else
		{
			thread t0(gotosector,0,sector_num);
			thread t1(gotosector,1,sector_num);
			thread t2(gotosector,2,sector_num);
			thread t3(gotosector,3,sector_num);
			thread t4(gotosector,4,sector_num);


			t0.join();
			t1.join();
			t2.join();
			t3.join();
			t4.join();

			int seektime=track-prevtrack;
			if(seektime<0)
				seektime=0-seektime;
			cout<< "Seektime : " << seektime <<"\n\n\n";
			totalseek+=seektime;
			prevtrack=track;
		}
	}
	cout<<"Total Seek : "<<totalseek;
	cout<<"\nCache Hits : "<<cachehit<<"\n";

    	std::cout.rdbuf(coutbuf); //reset to standard output again

}


