#include <fstream> //In Linux, each partition can be accessed as a file, hence fstream
#include <cerrno>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#define MAX_REQUESTS 1000000 
#define SECTOR_SIZE 512
#define SECTOR_UB 100000

int request[MAX_REQUESTS];
clock_t fcfs_total_time;
clock_t clook_total_time;

clock_t execrequests(){
	int sector = 0;
	int traversals = request[0];
	clock_t t; 

  	// Select partition
  	char diskName[] = "/dev/loop0";
  	std::string diskError = std::string() + diskName + ": ";

  	// Access partition
  	std::ifstream disk(diskName, std::ios_base::binary);

  	if(!disk)
    	throw(std::runtime_error(diskError + std::strerror(errno)));

	//disk.seekg (0, disk.end);
	//int length = disk.tellg(); // tellg() returns the distance from the beginning of the file
	//std::cout<<length;

	std::vector<char> buffer(SECTOR_SIZE);
	disk.clear();
	
	// Start clock
	t = clock();

	for(sector=0; sector<MAX_REQUESTS; sector++){
		
		// Add to traversals, total sectors covered till now
		traversals += abs(request[sector]-request[sector-1]); 
	  	
		// Seek to the sector
		disk.seekg(SECTOR_SIZE*request[sector]);
		if(!disk)
			throw(std::runtime_error(diskError + std::strerror(errno)));

		// Read in one sector
	  	disk.read(&buffer[0], SECTOR_SIZE);
	  	if(!disk)
			throw(std::runtime_error(diskError + std::strerror(errno)));
		
		//Print the sector 
		/*
		for(std::vector<char>::const_iterator i = buffer.begin(); i != buffer.end(); ++i)
			std::cout << *i;
		*/		
	}

	// Stop clock
	t = clock()-t;

	//std::cout <<"Time taken: " <<(float)t/CLOCKS_PER_SEC << "\n";
	return t;
}

void mergesort(int a[], int low, int high){
	int i = 0;
	int length = high - low + 1;
	int pivot  = 0;
	int merge1 = 0;
	int merge2 = 0;
	int working[length];

	if(low == high)
		return;

	pivot  = (low + high) / 2;

	mergesort(a, low, pivot);
	mergesort(a, pivot + 1, high);
 
	for(i = 0; i < length; i++)
		working[i] = a[low + i];

	merge1 = 0;
	merge2 = pivot - low + 1;

	for(i = 0; i < length; i++) {
		if(merge2 <= high - low)
			if(merge1 <= pivot - low)
				if(working[merge1] > working[merge2])
					a[i + low] = working[merge2++];
				else
					a[i + low] = working[merge1++];
			else
				a[i + low] = working[merge2++];
		else
			a[i + low] = working[merge1++];
	}
}

// Generate sector requests between 1 and SECTOR_UB
void genrequests(){
	srand(time(NULL));
	for(int i=0; i<MAX_REQUESTS; i++){
		request[i] = rand()%SECTOR_UB + 1;
	} 
}

void FCFS(){
	std::cout << "\n" << "Algorithm: FCFS";
	std::cout << "\n" << "---------------" << "\n";
	fcfs_total_time = execrequests();
	std::cout << "Clock cycles elapsed: " << fcfs_total_time << "\n";
}

void CLOOK(){
	std::cout << "\n" << "Algorithm: CLOOK";
	std::cout << "\n" << "----------------" << "\n";
	std::cout << "Starting to look from sector 1" << "\n";
	mergesort(request, 0, MAX_REQUESTS - 1);
	clook_total_time = execrequests();
	std::cout << "Clock cycles elapsed: " << clook_total_time << "\n";
}


void SSTF(){
	std::cout << "\n" << "Algorithm: SSTF";
	std::cout << "\n" << "----------------" << "\n";
	int reorder[MAX_REQUESTS];
	int current = MAX_REQUESTS/2;
	int total = 1;
	int left;
	int right;
	reorder[0] = request[current];
	while(total<MAX_REQUESTS){
		left = seekleft(current);
		right = seekright(current);
		if(abs(request[left]-request[current])<abs(request[left]-request[current])){
			reorder[total] = request[left];
			request[left] = -1;
			current = left;
		}
		else{
			reorder[total] = request[right];
			request[right] = -1;
			current = right;
		}
		total++;	
	}
}


int main(){
	genrequests();
	std::cout << "\n" << "General Details";
	std::cout << "\n" << "---------------" << "\n";
	int sectorsize = SECTOR_SIZE;
	std::cout << "All requests made to the sectors in the partition /dev/sda" << "\n";
	std::cout << "Sector size: " << sectorsize << "\n";
	int totalrequests = MAX_REQUESTS;
	std::cout << "Total requests: " << totalrequests << "\n";
	int upperbound = SECTOR_UB;
	std::cout << "Sectors requested randomly between 1 and " << upperbound << "\n";
	FCFS();
	CLOOK();
	std::cout << "\nCLOOK takes "<< (float)clook_total_time*100/fcfs_total_time << "% of time compared to FCFS\n\n";
}
