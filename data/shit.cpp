#include "utillib.h"

int main(int argc, char *argv[])
{
	CFileExportContext out;
	float buckets[100] = {
	   0.0,     1.0,     5.0,     10.0, 
		   20.0,    50.0,    100.0, 
		  500.0,  1000.0,  10000.0, 
		25000.0, 50000.0, 100000.0, 9999999999.0
	};
	SFInt32 nBuckets = 0;
	SFInt32 counts[100];
	float totals[100];
	for (int i=0;i<100;i++)
	{
		counts[i] = 0;
		totals[i] = 0.0;
	}

	SFString contents = asciiFileToString("data.txt");
	while (!contents.IsEmpty())
	{
		SFString line = nextTokenClear(contents, '\n');
		float val = atof((const char*)line);
printf("%ld ",     nBuckets);
printf("%10.5f ",  val);
printf("%10.5f ",  buckets[nBuckets]);
printf("%10.5f\n", buckets[nBuckets+1]);

		if (val < buckets[nBuckets+1])
			nBuckets++;
		counts[nBuckets]++;
		totals[nBuckets] += val;
	}

	printf("Results: %ld buckets\n", nBuckets);
	for (int i=0;i<nBuckets;i++)
		printf("%ld\t%10.5f\t%ld\t%10.5f\n", i, buckets[i], counts[i], totals[i]);
	return 0;
}
