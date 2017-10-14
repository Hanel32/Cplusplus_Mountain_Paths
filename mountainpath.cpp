#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

void findMinMax(const vector<float>& data, int& min, int& max){
	min = data.at(0);
	max = min;
	
	for(int i = 1; i < data.size(); i++){
		if(data.at(i) < min){
			min = data.at(i);
		}
		else if(data.at(i) > max){
			max = data.at(i);
		}
	}
}
void convertScale(vector<float>& data, const int& min, const int& max){
	for(int i = 0; i < data.size(); i++){
			data.at(i) = ((data.at(i) - min) / (max - min)) * 255.;
	}
}
void populate(const vector<float>& data, vector<vector<int> >& red, int row, int col){
	int c = 0;                                //index value for data, because it is a singular dimension.
	
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			red.at(i).at(j) = data.at(c);
			c++;
		}
	}
}	
int checkNext(int row, int col, const vector<vector<int> >& data, int rowSize){
	float up, dow, acc;
	up = dow = acc = 214700000.;

	if(col < data[0].size()){
		if(row > 0){
			up = abs(data.at(row).at(col) - data.at(row-1).at(col+1));
		}
		acc = abs(data.at(row).at(col) - data.at(row).at(col+1));
		if(row < rowSize-1){
			dow = abs(data.at(row).at(col) - data.at(row+1).at(col+1));
		}
	//	if(row > 300){
		//	cout << "Up value:   " << up << endl;
		//	cout << "Down value: " << dow << endl;
			//cout << "Acc  value: " << acc << endl;
		//}
		if(acc <= up && acc <= dow){
		//	if(row > 300)
				//cout << "Acc was chosen" << endl;
			return 0;
		}
		if(dow <= up && dow <= acc){
		//	if(row > 300)
				//cout << "Dow was chosen" << endl;
			return 1;
		}
		if(up <= acc && up <= dow){
		//	if(row > 300)
				//cout << "Up was chosen" << endl;
			return -1;
		}
	}
}
int colorPath(const vector<vector<int> >& heightMap, vector<vector<int> >& r,vector<vector<int> >& g, 
vector<vector<int> >& b, int color_r, int color_g, int color_b, int start_row){
	int dir, iter, length;
	dir = iter = length = 0;
	
	while(iter < heightMap[0].size()){
		r.at(start_row).at(iter) = color_r;
		g.at(start_row).at(iter) = color_g;
		b.at(start_row).at(iter) = color_b;
		if(iter < heightMap[0].size()-1){
			dir += checkNext(start_row, iter, heightMap, heightMap.size());
			length += abs(heightMap.at(start_row).at(iter) - heightMap.at(start_row+dir).at(iter+1)); //Adds the change in elevation to the vector.
		}
		start_row += dir;
		iter++;
		dir = 0;
	}
return length;	
}
int findLowest(vector<int> data){
	int lowNum = data.at(0);
	int lowInd;
	
	for(int i = 1; i < data.size(); i++){
		if(data.at(i) < lowNum){
			lowNum = data.at(i);
			lowInd = i;
		}
	}
	return lowInd;
}
int main(){
	vector<float> data;
	vector<int> length;
	vector<vector<int> > gre, blu, data_orig;
	int max, min, row, col, lowInd;
	double temp;
	string file;
	
	//cin >> row >> col >> file;
	row = 480;
	col = 844;
	
	//ifstream inFS(file);
	ifstream inFS("map-input-844-480.dat");
	if(!inFS.is_open()){
		cout << "Could not open file" << endl;
	}
	else{
		while(!inFS.eof()){                     //Getting initial data
			inFS >> temp;
			data.push_back(temp); 
		}
		inFS.close();                           //Closing file stream after getting data
		vector<vector<int> > red (row, vector<int> (col, 0));
		data_orig = red;
		findMinMax(data, min, max);
		populate(data, data_orig, row, col);    //Puts the original elevation data into a 2d vector to hold values for reference
		convertScale(data, min, max);
		populate(data, red, row, col);
		//file += ".ppm";
		//ofstream outFS(file);
		ofstream outFS("out.ppm");
		outFS << "P3" << endl << col << endl << row << endl << 255 << endl;
	    blu = gre = red;
	    system("pause");
		for(int i = 0; i < row; i++){
			length.push_back(colorPath(data_orig, red, gre, blu, 252, 63, 25, i));
		}
		lowInd = findLowest(length);
		colorPath(data_orig, red, gre, blu, 31, 13, 253, lowInd);
		for(int i = 0; i < row; i++){
			for(int j = 0; j < col; j++){
				outFS << red.at(i).at(j) << " " << blu.at(i).at(j) << " " << gre.at(i).at(j) << " ";
			}
		}
		cout << "Pixel (843, 39) expected: 31, 253, 13 ; Actual: " << red.at(39).at(843) << endl;
		outFS.close();
	}
	return 0;
}
