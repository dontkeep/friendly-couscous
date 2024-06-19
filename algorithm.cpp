#include <iostream> //for reading a file
#include <sstream>
#include <fstream>
#include <cstdlib> 
#include <ctime> // for random seeding
#include <vector>

using namespace std;

//creating a Point struct

struct Point
{
  double x, y; //coordinates
  int cluster; // number of clusters will be used (no default cluster)
  double minDistance; // minimum distance to nearest cluster (default infinite)

  Point():
    x(0.0),
    y(0.0),
    cluster(-1),
    minDistance(__DBL_MAX__){} // initialisation list cause we set the default value from the struct, __DBL_MAX__ is the next best thing to infinity so we'll use that 
  
  Point(double x, double y):
    x(x),
    y(y),
    cluster(-1),
    minDistance(__DBL_MAX__){}

  double distance(Point p) {
    return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y); // euclidean distance formula
  }
};
  
// now we can create a point object from struct, let's make a function to read a csv file now
// reading csv file 
vector<Point> readcsv() {
  vector<Point> points;
  string line;
  ifstream file("new_mall_customers.csv");

  //loop through every line inside the file and put it inside points as a vector type struct Point
  while (getline(file, line)) {
      stringstream lineStream(line);
      string bit;
      double x, y;
      getline(lineStream, bit, ',');
      x = stof(bit);
      getline(lineStream, bit, '\n');
      y = stof(bit);

      points.push_back(Point(x, y));
  }
  return points;
}

// now that we have a function that reads a csv file and returns a vector, we're gonna start making the kmeans clustering function
// the function, of course we must input the points, epochs for the number of iteration, and k for the number of clusters
void kMeansClustering(vector<Point>* points, int epochs, int k) {
  //create a centroids type vector Points
  vector<Point> centroids;

  //create the total number of points
  vector<int> nPoints;
  
  vector<double> sumX, sumY;
  
  //dereference the points and the size of the points will be used to create the centroids
  size_t n = points->size();
  
  srand(time(0));  // need to set the random seed
  
  for (int i = 0; i < k; ++i) {
    centroids.push_back(points->at(rand() % n));
  }

  // assigning points to the cluster
  for (vector<Point>::iterator c = begin(centroids); 
     c != end(centroids); ++c) {
    // quick hack to get cluster index
    int clusterId = c - begin(centroids);

    for (vector<Point>::iterator it = points->begin();
         it != points->end(); ++it) {
             
        Point p = *it;
        double dist = c->distance(p);
        if (dist < p.minDistance) {
            p.minDistance = dist;
            p.cluster = clusterId;
        }
        *it = p;
    }
  }
  for (int j = 0; j < k; ++j) {
    nPoints.push_back(0);
    sumX.push_back(0.0);
    sumY.push_back(0.0);
  }
  // Iterate over points to append data to centroids
  for (vector<Point>::iterator it = points->begin(); 
     it != points->end(); ++it) {
    int clusterId = it->cluster;
    nPoints[clusterId] += 1;
    sumX[clusterId] += it->x;
    sumY[clusterId] += it->y;

    it->minDistance = __DBL_MAX__;  // reset distance
  }

  // Compute the new centroids
  for (vector<Point>::iterator c = begin(centroids); 
      c != end(centroids); ++c) {
      int clusterId = c - begin(centroids);
      c->x = sumX[clusterId] / nPoints[clusterId];
      c->y = sumY[clusterId] / nPoints[clusterId];
  }

  ofstream myfile;
  myfile.open("output.csv");
  myfile << "x,y,c" << endl;

  for (vector<Point>::iterator it = points->begin(); 
      it != points->end(); ++it) {
      myfile << it->x << "," << it->y << "," << it->cluster << endl;
      cout<<it->x << "," << it->y << "," << it->cluster << endl;
  }
  myfile.close();
}; 
  
int main (){
  vector<Point> points = readcsv(); 
  kMeansClustering(&points, 150, 5);
}

//reference : https://reasonabledeviations.com/2019/10/02/k-means-in-cpp/
