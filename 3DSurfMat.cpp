#include <iostream>
#include <cstdio>
#include <fstream>
#include <cfloat>
#include <cstdlib>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;
class TDSurfMat
{
public:
	//vector<double> yVector;
	//vector< vector<double> > xVector;
	vector< vector<double> > SurfVector;
	
	TDSurfMat()
	{}
	void readDataFile(const string &fn);
	void SmoothAveFunct(int cubeSize, int interTime);
	void WriteSmoothData(const string &fw);
private:
};

void TDSurfMat::readDataFile(const string &fn)
{
	cout<<"==READING MATRIX FROM "<<fn<<"=="<<endl;
	fstream inpfile(fn.c_str());
	
	if(!inpfile)
        {   
                cout <<"Error!"<<"Failed In Open "<<fn<<endl;
                abort();
        }

	vector<double> jVector;	
	string line;
	int iiline=0;
	int jjline=0;
	double xx, yy, zz;
	bool ptfile=false;
	
	while(getline(inpfile, line))
	{
		//jjline++;

		if(line.length()<=5)
		{
			//new line 
			//iiline++;
			//jjline=0;
			//continue;
			SurfVector.push_back(jVector);
			jVector.clear();
		}
		else
		{
			//old line
			istringstream str(line);
			str >> xx >> yy >> zz;
			jVector.push_back(zz);
			//cout << "line:" << SurfVector.size() << " index:" << jVector.size() << endl;
		}

		//cout<<line<<"    "<<line.length()<<endl;
		//istringstream str(line);
		//str >> xx >> yy >> zz;
		//cout << xx<<"  "<< yy<<"  "<< zz<<"  "<<endl;
		//yVector.push_back(zz);
		//cout << xx<<"  "<< yy<<"  "<< zz<<"  "<<yVector[nnline-1]<<"  "<<endl;
		//SurfVector[iiline].push_back(zz);
		//cout << xx<<"  "<< yy<<"  "<< zz<<"  "<<SurfVector[iiline][jjline-1]<<"  "<<endl;
		//cout << "line:" << SurfVector.size() << " index:" << jVector.size() << endl;
	}
	
	cout<<"==END READING MATRIX  "<<fn<<"=="<<endl;
	
}


//Smoothing By average method.
//The cube (i-cubeSize, j-cubeSize) to (i+cubeSize, j+cubeSize) will be averaged;
// |------------|
// |  (ii, jj)  |
// |------------|
void TDSurfMat::SmoothAveFunct(int cubeSize, int interTime)
{
	int ii, jj, kk;
	int i, j;
	int PMinii, Pmaxii; //define a cube cell
	int PMinjj, Pmaxjj;
	double AveVal;
	cout <<"line:" <<SurfVector.size() << " index:"<<SurfVector[0].size()<<endl;
	int MinVecComp;
	MinVecComp=SurfVector.size() < SurfVector[0].size() ? SurfVector.size() : SurfVector[0].size();
	//cout <<MinVecComp<<endl;
	cubeSize= (2*cubeSize+1) < MinVecComp ? cubeSize : MinVecComp/2;
	//cout << cubeSize <<endl;
	
	//Smoothing
	for (kk=0; kk<= (interTime-1); kk++)
	{
		cout<<"==SMOOTHING THE "<<kk<<"th  TIME=="<<endl;
		for (ii=0; ii<=(SurfVector.size()-1); ii++)	
		{
			PMinii=0 <= (ii-cubeSize) ? (ii-cubeSize) : 0;
			Pmaxii=SurfVector.size() <= (ii+cubeSize) ? (SurfVector.size()-1) : (ii+cubeSize);
			//cout <<"SurfVector["<<ii<<"].size() = " << SurfVector[ii].size()-1<<endl; 
			//
			for (jj=0; jj<=(SurfVector[ii].size()-1); jj++)
			{
				//cout << kk<< "  "<< ii<<"  "<< jj<<endl;
				PMinjj=0 <= (jj-cubeSize) ? (jj-cubeSize) : 0;
				Pmaxjj=SurfVector[ii].size() <= (jj+cubeSize) ? (SurfVector[ii].size()-1) : (jj+cubeSize);
				//cout << kk<< "  "<< ii<<"  "<< jj<<"  "<< PMinii <<"  "<< Pmaxii<<"  "<< PMinjj<<"  "<< Pmaxjj<<endl;

				AveVal=0;
				for (i=PMinii; i<=Pmaxii; i++)
				{
					for (j=PMinjj; j<=Pmaxjj; j++)
					{
						AveVal=AveVal+SurfVector[i][j];	
					}
				}	
				AveVal=AveVal/(1.0*(Pmaxii-PMinii+1)*(Pmaxjj-PMinjj+1));
				//cout<<"test: AveVal = "<<AveVal<<endl;
				SurfVector[ii][jj]=AveVal;
			}
		}
	}
}

//Ooutput smoothed data
void TDSurfMat::WriteSmoothData(const string &fw)
{
	cout<<"==      WRITING DATA FILE     =="<<endl;
	fstream outfile(fw.c_str(), ios::out);
	if(!outfile)
        {
                cout<<"==Failed in OPEN/CREATE file: OUT.psf=="<<endl;
        }

	int i, j;
	for (i=0; i<SurfVector.size(); i++)	
	{
		for (j=0; j<SurfVector[i].size(); j++)
		{
			outfile<<setw(9)<<setprecision(4)<<setiosflags(ios::fixed)<<SurfVector[i][j]<<"\n";	
		}
		outfile<<" "<<"\n";
	}
	cout<<"==    END WRITING DATA FILE   =="<<endl;
}
int main()
{	
	TDSurfMat aa;
	string file;
	file="V.final.grid.out";
	aa.readDataFile(file);
	aa.WriteSmoothData("1");
	aa.SmoothAveFunct(1, 90);
	aa.WriteSmoothData("2");
}
