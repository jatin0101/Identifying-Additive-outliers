#include<bits/stdc++.h>
#include<cmath>
using namespace std;

#define N 10000						// Total Number of points in textfile.txt, dont change it!

float* get_my_file()				// Function to read textfile.txt
{
	float* a; 
	a = new float [N];
	fstream newfile;
	newfile.open("testfile.txt",ios::in);

   	if (newfile.is_open())
   	{   
		string tp;
		int i=0;
		while(getline(newfile, tp))
		{
			a[i]=stof(tp);
			i++;
		}
		newfile.close();
  	}
  	else
  	{
  		cout<<"File not read!!\n";
  	}

  return a;

}
//First method to find anomalies --- Bollinger Bands Method
int method_Bollinger(float *a,int *anomaly)				
{
	int count=0;
	//Setting the threshold to be 2.7 so that the number of anomalies is appropriate.
	float threshold=2.7;								
	float mean[N],sd[N];
	for(int i=9;i<N;i++)
	{
		float sum=0,sum_sd=0;							
		for(int j=i;j>i-10;j--) 
		{
			sum+=a[j];
			sum_sd+=a[j]*a[j];
		}
		//mean and standard deviation can also be calculated in separate functions
		mean[i]=sum/10;
		sd[i]=sqrt(sum_sd/10-(mean[i]*mean[i]));
		//checking the condition. If the data value dosen't lie within the limits, then it is an anomaly point.
		if(a[i]<mean[i]-threshold*sd[i]||a[i]>mean[i]+threshold*sd[i]) anomaly[count++]=i;			
	}
	return count;
}

//Second method----Slope method
int method_slope(float *a,int *anomaly)					
{														
	int count=0;
	float slope[N];
	//defining slope
	//every point has two slopes related to it----one with previous one and the other with the next value.
	for(int i=1;i<N;i++) slope[i]=a[i]-a[i-1];			
	float sum=0,sum_sd=0;								
	for(int j=1;j<N;j++) 
	{
		sum+=slope[j];
		sum_sd+=slope[j]*slope[j];
	}
	float mean=sum/(N-1);
	float sd=sqrt(sum_sd/(N-1)-(mean*mean));
	for(int j=1;j<N;j++)
		if(((slope[j]<mean-3.5*sd)||(slope[j]>mean+3.5*sd))&&(slope[j+1]<mean-3.5*sd||slope[j+1]>mean+3.5*sd))	
		//The second condition (after the && operator) is given to remove those anomaly points which has one slope within limits and the other one outside the limits.	
		//For index-106 slope with the previous value lies within the limit but the slope with the next value is outside the limits.	
		//This method will remove such anomaly points. If the second condition (after the && operator)is removed, index-107 will be shown as an anomaly point.
			anomaly[count++]=j++;	 
			//(Using j++ so that the value next to an anomaly point is not affected.)		 
	return count;																									
}

 //Third method ----- This is my method.
 //This method might not be accurate.
 //This method uses the ratio of the data values to identify the anomaly points.
 //While using ratio, we have to use moving average and standard deviation of the ratios to set the limits.
int mymethod(float *a,int *anomaly)					   
{														
	int count=0;			
	//Setting the threshold to be 2.9 so that the number of anomalies is appropriate.							
	float ratio[N],b[N],threshold=2.9;	
	//While using ratio, we have to ensure that the denominator is not 0.
	//Just to ensure that the ratio is valid, I assumed 0 to be equal to 0.001.				
	for(int i=0;i<N;i++) 
	{								
		if(a[i]==0) 
		{
			b[i]=0.001;
			continue;
		}
		b[i]=a[i];
	}
	for(int i=1;i<N;i++) ratio[i]=b[i]/b[i-1]; 
	float mean[N],sd[N];
	for(int i=9;i<N;i++)
	{													
		//Calculating moving mean and standard deviation
		float sum=0,sum_sd=0;
		for(int j=i;j>i-10;j--) 
		{
			sum+=ratio[j];
			sum_sd+=ratio[j]*ratio[j];
		}
		mean[i]=sum/10;
		sd[i]=sqrt(sum_sd/10-(mean[i]*mean[i]));		
	}													
	//Setting the appropriate limits.
	//Using i++ so that the value next to an anomaly point is not affected.
	for(int i=9;i<N;i++)
	    if(ratio[i]>mean[i]+threshold*sd[i]||ratio[i]<mean[i]-threshold*sd[i])
	        anomaly[count++]=i++;
	return count;										
}

int main()
{
	float* input = get_my_file();
	/*Uncomment the code below to check if the file is successfully printed to the console
	auto temp = input;
	for(int i = 0; i < N; i++)
	{
		cout<<*temp<<endl;									
		temp++;
	}*/
	
	int anomaly_list[N],num_anomaly;			
	//Printing the anomaly indexes.
	
	//Printing the anomaly indexes detected by Bollinger Bands method.
	cout<<"The anomaly indexes detected using Bollinger Bands method are:"<<endl;
	num_anomaly=method_Bollinger(input,anomaly_list);
	for(int j=0;j<num_anomaly;j++) cout<<anomaly_list[j]<<endl; 
	
	//Printing the anomaly indexes detected by the slope method.
	cout<<endl<<"The anomaly indexes detected using Slope method are:"<<endl;
	num_anomaly=method_slope(input,anomaly_list);
	for(int j=0;j<num_anomaly;j++) cout<<anomaly_list[j]<<endl;
	
	//Printing the anomaly indexes detected by my method.
	cout<<endl<<"The anomaly indexes detected using my method (using ratios) are:"<<endl;
	num_anomaly=mymethod(input,anomaly_list);
	for(int j=0;j<num_anomaly;j++) cout<<anomaly_list[j]<<endl; 
	
	//If we have to print the corresponding data values also, along with the indexes, then cout<<anomaly_list[j]<<"  "<<input[anomaly_list[j]]<<endl; statement can be used.	
}   