// Julia Ciattei CMSC341 Project 3
// Test file for the PQueue class

// provided implementation including priority functions
// and Random class /////////////////////////////////////////////////////

#include "pqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

// Priority functions compute an integer priority for a patient.  Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Patient & patient);
int priorityFn2(const Patient & patient);

// We can use the Random class to generate the test data randomly!
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

int priorityFn1(const Patient & patient) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [115-242]
    //temperature + respiratory + blood pressure
    //the highest priority would be 42+40+160 = 242
    //the lowest priority would be 35+10+70 = 115
    //the larger value means the higher priority
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}

int priorityFn2(const Patient & patient) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [71-111]
    //nurse opinion + oxygen
    //the highest priority would be 1+70 = 71
    //the lowest priority would be 10+101 = 111
    //the smaller value means the higher priority
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}

/////////////////////////////////////////////////////////////////////////////////////

// case types
const string NORM = "Normal";
const string EDGE = "Edge";
const string ERROR = "Error";

// results
const string PASS = "Passed";
const string FAIL = "Failed";

// testing contants
const int NUM_PATIENTS = 300;

class Tester{
public:
  // test normal cases for overloaded constructor
  bool constructNormal1();
  bool constructNormal2();
  
  // test normal cases for insert
  bool insertNormalMinLeftist();
  bool insertNormalMaxLeftist();
  bool insertNormalNPL();
  bool insertNormalMinSkew();
  bool insertNormalMaxSkew();

  // test error case for insert
  bool insertErrorEmpty();

  // test clear
  bool clearNormal();
  bool clearError();
  
  // test normal cases for remove
  bool removeNormalMinLeftist();
  bool removeNormalMaxLeftist();
  bool removeNormalMinSkew();
  bool removeNormalMaxSkew();

  // test error case for remove
  bool removeErrorEmpty();

  // test normal cases for mergeWithQueue
  bool mergeNormalMinLeftist();
  bool mergeNormalMaxLeftist();
  bool mergeNormalMinSkew();
  bool mergeNormalMaxSkew();

  // test edge cases for mergeWithQueue
  bool mergeEdgeNonEmptytoEmpty();
  bool mergeEdgeEmptytoNonEmpty();

  // test error cases for mergeWithQueue
  bool mergeErrorSelf();
  bool mergeErrorDiffTypes();
  
  // test normal cases for copy constructor
  bool copyNormalMinLeftist();
  bool copyNormalMaxLeftist();
  bool copyNormalMinSkew();
  bool copyNormalMaxSkew();

  // test edge case for copy constructor
  bool copyEdgeEmpty();
  
  // test normal cases for setPriorityFn
  bool setPriorityFnNormalMinLeftist();
  bool setPriorityFnNormalMaxLeftist();
  bool setPriorityFnNormalMinSkew();
  bool setPriorityFnNormalMaxSkew();

  // test normal cases for setPriorityFn
  bool setStructureNormalMinLeftist();
  bool setStructureNormalMaxLeftist();
  bool setStructureNormalMinSkew();
  bool setStructureNormalMaxSkew();

  // test normal cases for assignment operator
  bool assignNormalMinLeftist();
  bool assignNormalMaxLeftist();
  bool assignNormalMinSkew();
  bool assignNormalMaxSkew();

  // test edge case for assignment operator
  bool assignEdgeToEmpty();
  
  // helper to display the result of the test
  void resultMsg(string caseType, string methodName, string result);

private:
  // helper to generate a vector of patients
  void randPatients(int numPatients, vector<Patient> &patients);
  // check if the queue maintains heap properties
  bool isHeap(const PQueue &queue);
  bool isHeap(Node* node, const HEAPTYPE &heapType, const prifn_t &priorFunc);
  // check if two patients are equal
  bool isEqualPatient(Patient patient1, Patient patient2);
  // confirm correct NPL for a leftist heap
  bool isNPL(const PQueue &queue);
  bool isNPL(Node* node);
  
};

// test normal case for overloaded constructor
bool Tester::constructNormal1(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing overloaded constructor - Fn2, min, leftist:\n";
  // did the properties initialize
  if (pqueue.m_heap == nullptr && pqueue.m_size == 0
      && pqueue.m_priorFunc == priorityFn2
      && pqueue.m_heapType == MINHEAP
      && pqueue.m_structure == LEFTIST){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for overloaded constructor
bool Tester::constructNormal2(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, SKEW);
  cout << "Testing overloaded constructor - Fn1, max, skew:\n";
  // did the properties initialize
  if (pqueue.m_heap == nullptr && pqueue.m_size == 0
      && pqueue.m_priorFunc == priorityFn1
      && pqueue.m_heapType == MAXHEAP
      && pqueue.m_structure == SKEW){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for insert - min leftist heap
bool Tester::insertNormalMinLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing insertPatient(...) - min leftist heap:\n";
  
  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);
  
  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }
  
  // confirm that heap properties are maintained
  if (isHeap(pqueue) && pqueue.m_size == NUM_PATIENTS){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for insert - max leftist heap
bool Tester::insertNormalMaxLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, LEFTIST);
  cout << "Testing insertPatient(...) - max leftist heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // confirm that heap properties are maintained
  if (isHeap(pqueue) && pqueue.m_size == NUM_PATIENTS){
    return true;
  }
  else{
    return false;
  }
}

// test NPL values of a leftist heap
bool Tester::insertNormalNPL(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing insertPatient(...) - NPL values in leftist heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(50, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }
  
  // confirm that heap properties are maintained
  if (isNPL(pqueue) && pqueue.m_size == 50){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for insert - min skew heap
bool Tester::insertNormalMinSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, SKEW);
  cout << "Testing insertPatient(...) - min skew heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // confirm that heap properties are maintained
  if (isHeap(pqueue) && pqueue.m_size == NUM_PATIENTS){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for insert - max skew heap
bool Tester::insertNormalMaxSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, SKEW);
  cout << "Testing insertPatient(...) - max skew heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // confirm that heap properties are maintained
  if (isHeap(pqueue) && pqueue.m_size == NUM_PATIENTS){
    return true;
  }
  else{
    return false;
  }
}

// test error case for insert
bool Tester::insertErrorEmpty(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing insertPatient(...) - attempt to insert empty patient:\n";

  Patient patient = Patient();
  pqueue.insertPatient(patient);

  // heap should be empty
  if (pqueue.m_heap == nullptr){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for clear
bool Tester::clearNormal(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing clear() - clear non-empty heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  pqueue.clear();
  
  // confirm that heap properties are maintained
  if (pqueue.m_heap == nullptr && !pqueue.m_size){
    return true;
  }
  else{
    return false;
  }
}

// test error case for clear
bool Tester::clearError(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing clear() - clear empty heap:\n";

  pqueue.clear();

  // confirm that heap properties are maintained
  if (pqueue.m_heap == nullptr && !pqueue.m_size){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for getNextPatient - min leftist heap
bool Tester::removeNormalMinLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing getNextPatient(...) - min leftist heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }
  
  bool isRemoved = true; // assume the function works
  Patient nextPatient;
  Patient removed;
  
  // remove all patients from heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    // ensure that this patient gets removed
    nextPatient = pqueue.m_heap->m_patient;
    removed = pqueue.getNextPatient();
    
    // was the correct patient removed and heap maintained
    if (!isEqualPatient(nextPatient, removed)
	|| !isHeap(pqueue)){
      isRemoved = false;
    }
  }

  // confirm that removals were done correctly
  if (isRemoved){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for getNextPatient - max leftist heap
bool Tester::removeNormalMaxLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, LEFTIST);
  cout << "Testing getNextPatient(...) - max leftist heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  bool isRemoved = true; // assume the function works
  Patient nextPatient;
  Patient removed;

  // remove all patients from heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    // ensure that this patient gets removed
    nextPatient = pqueue.m_heap->m_patient;
    removed = pqueue.getNextPatient();

    // was the correct patient removed and heap maintained
    if (!isEqualPatient(nextPatient, removed)
        || !isHeap(pqueue)){
      isRemoved = false;
    }
  }

  // confirm that removals were done correctly
  if (isRemoved){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for getNextPatient - min skew heap
bool Tester::removeNormalMinSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, SKEW);
  cout << "Testing getNextPatient(...) - min skew heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  bool isRemoved = true; // assume the function works
  Patient nextPatient;
  Patient removed;

  // remove all patients from heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    // ensure that this patient gets removed
    nextPatient = pqueue.m_heap->m_patient;
    removed = pqueue.getNextPatient();

    // was the correct patient removed and heap maintained
    if (!isEqualPatient(nextPatient, removed)
        || !isHeap(pqueue)){
      isRemoved = false;
    }
  }

  // confirm that removals were done correctly
  if (isRemoved){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for getNextPatient - max skew heap
bool Tester::removeNormalMaxSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, SKEW);
  cout << "Testing getNextPatient(...) - max skew heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  bool isRemoved = true; // assume the function works
  Patient nextPatient;
  Patient removed;

  // remove all patients from heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    // ensure that this patient gets removed
    nextPatient = pqueue.m_heap->m_patient;
    removed = pqueue.getNextPatient();

    // was the correct patient removed and heap maintained
    if (!isEqualPatient(nextPatient, removed)
        || !isHeap(pqueue)){
      isRemoved = false;
    }
  }

  // confirm that removals were done correctly
  if (isRemoved){
    return true;
  }
  else{
    return false;
  }
}

// test error case for remove - removing from an empty heap
bool Tester::removeErrorEmpty(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing getNextPatient(...) - removing from an empty heap:\n";

  // this should throw an exception
  try{
    pqueue.getNextPatient();
  }
  catch (exception &error){
    cout << error.what();
    return true;
  }
  return false;
}

// test normal cases for mergeWithQueue - min leftist
bool Tester::mergeNormalMinLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing mergeWithQueue(...)- min leftist:\n";
  vector<Patient> patients1;
  randPatients(10, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }
  
  // instantiate another heap
  vector<Patient> patients2;
  randPatients((10 / 2), patients2);
  PQueue pqueue1 = PQueue(priorityFn2, MINHEAP, LEFTIST);
  for (auto it = patients2.begin(); it < patients2.end(); ++it){
    pqueue1.insertPatient(*it);
  }
  
  // merge heaps
  pqueue1.mergeWithQueue(pqueue);
  
  // confirm that heap properties are maintained
  if (isHeap(pqueue1) && pqueue1.m_size == 15 && pqueue.m_heap == nullptr){
    return true;
  }
  else{
    return false;
  }
}

// test normal cases for mergeWithQueue - max leftist
bool Tester::mergeNormalMaxLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, LEFTIST);
  cout << "Testing mergeWithQueue(...)- max leftist:\n";
  vector<Patient> patients1;
  randPatients(10, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // instantiate another heap
  vector<Patient> patients2;
  randPatients((10 / 2), patients2);
  PQueue pqueue1 = PQueue(priorityFn1, MAXHEAP, LEFTIST);
  for (auto it = patients2.begin(); it < patients2.end(); ++it){
    pqueue1.insertPatient(*it);
  }

  // merge heaps
  pqueue1.mergeWithQueue(pqueue);

  // confirm that heap properties are maintained
  if (isHeap(pqueue1) && pqueue1.m_size == 15 && pqueue.m_heap == nullptr){
    return true;
  }
  else{
    return false;
  }
}

// test normal cases for mergeWithQueue - min skew
bool Tester::mergeNormalMinSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, SKEW);
  cout << "Testing mergeWithQueue(...)- min skew:\n";
  vector<Patient> patients1;
  randPatients(10, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // instantiate another heap
  vector<Patient> patients2;
  randPatients((10 / 2), patients2);
  PQueue pqueue1 = PQueue(priorityFn2, MINHEAP, SKEW);
  for (auto it = patients2.begin(); it < patients2.end(); ++it){
    pqueue1.insertPatient(*it);
  }

  // merge heaps
  pqueue1.mergeWithQueue(pqueue);

  // confirm that heap properties are maintained
  if (isHeap(pqueue1) && pqueue1.m_size == 15 && pqueue.m_heap == nullptr){
    return true;
  }
  else{
    return false;
  }
}

// test normal cases for mergeWithQueue - max skew
bool Tester::mergeNormalMaxSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, SKEW);
  cout << "Testing mergeWithQueue(...)- max skew:\n";
  vector<Patient> patients1;
  randPatients(10, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // instantiate another heap
  vector<Patient> patients2;
  randPatients((10 / 2), patients2);
  PQueue pqueue1 = PQueue(priorityFn1, MAXHEAP, SKEW);
  for (auto it = patients2.begin(); it < patients2.end(); ++it){
    pqueue1.insertPatient(*it);
  }

  // merge heaps
  pqueue1.mergeWithQueue(pqueue);

  // confirm that heap properties are maintained
  if (isHeap(pqueue1) && pqueue1.m_size == 15 && pqueue.m_heap == nullptr){
    return true;
  }
  else{
    return false;
  }
}

// test edge case for mergeWithQueue - non-empty to empty
bool Tester::mergeEdgeNonEmptytoEmpty(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing mergeWithQueue(...)- merging non-empty to empty heap:\n";
  vector<Patient> patients1;
  randPatients(10, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }
    
  // instantiate another heap
  PQueue pqueue1 = PQueue(priorityFn2, MINHEAP, LEFTIST);
 
  // merge heaps
  pqueue1.mergeWithQueue(pqueue);
  
  // confirm that heap properties are maintained
  if (isHeap(pqueue1) && pqueue1.m_size == 10 && pqueue.m_heap == nullptr){
    return true;
  }
  else{
    return false;
  }
}

// test edge case for mergeWithQueue - empty to non-empty
bool Tester::mergeEdgeEmptytoNonEmpty(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing mergeWithQueue(...)- merging empty to non-empty heap:\n";
  vector<Patient> patients1;
  randPatients(10, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // instantiate another heap
  PQueue pqueue1 = PQueue(priorityFn2, MINHEAP, LEFTIST);

  // merge heaps
  pqueue.mergeWithQueue(pqueue1);

  // confirm that heap properties are maintained
  if (isHeap(pqueue) && pqueue.m_size == 10 && pqueue1.m_heap == nullptr){
    return true;
  }
  else{
    return false;
  }
}

// test error case for mergeWithQueue - self-merge
bool Tester::mergeErrorSelf(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing mergeWithQueue(...)- attempting merge heap to itself:\n";
  vector<Patient> patients1;
  randPatients(10, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // attempt to merge heaps
  pqueue.mergeWithQueue(pqueue);
  
  // confirm that heap properties are maintained
  if (isHeap(pqueue) && pqueue.m_heap != nullptr){
    return true;
  }
  else{
    return false;
  }
}

// test error case for mergeWithQueue - merging different types
bool Tester::mergeErrorDiffTypes(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing mergeWithQueue(...)- merging max to min leftist:\n";
  vector<Patient> patients1;
  randPatients(10, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // instantiate another heap
  PQueue pqueue1 = PQueue(priorityFn1, MAXHEAP, LEFTIST);

  // merge heaps - should throw an exception
  try{
    pqueue.mergeWithQueue(pqueue1);
  }
  catch (exception &error){
    cout << error.what();
    return true;
  }
  return false;
}


// test normal case for copy constructor - min leftist
bool Tester::copyNormalMinLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing copy constuctor- min leftist heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // copy the queue
  PQueue pqueue1 = PQueue(pqueue);
  
  // confirm that heap properties are maintained
  if (isHeap(pqueue1)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for copy constructor - max leftist
bool Tester::copyNormalMaxLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, LEFTIST);
  cout << "Testing copy constuctor- max leftist heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // copy the queue
  PQueue pqueue1 = PQueue(pqueue);

  // confirm that heap properties are maintained
  if (isHeap(pqueue1)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for copy constructor - min skew
bool Tester::copyNormalMinSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, SKEW);
  cout << "Testing copy constuctor- min skew heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // copy the queue
  PQueue pqueue1 = PQueue(pqueue);

  // confirm that heap properties are maintained
  if (isHeap(pqueue1)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for copy constructor - max skew
bool Tester::copyNormalMaxSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, SKEW);
  cout << "Testing copy constuctor- max skew heap:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // copy the queue
  PQueue pqueue1 = PQueue(pqueue);

  // confirm that heap properties are maintained
  if (isHeap(pqueue1)){
    return true;
  }
  else{
    return false;
  }
}

// test edge case for copy constructor
bool Tester::copyEdgeEmpty(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing copy constuctor- copy an empty heap:\n";

  // copy the queue
  PQueue pqueue1 = PQueue(pqueue);

  // confirm that heap properties are maintained
  if (isHeap(pqueue1)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for setPriority - leftist min to max
bool Tester::setPriorityFnNormalMinLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing setPriorityFn- leftist heap -> min to max:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(7, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // reset the heap type
  pqueue.setPriorityFn(priorityFn1, MAXHEAP);
  
  // confirm that heap properties are maintained
  if (pqueue.getPriorityFn() == priorityFn1
      && pqueue.getHeapType() == MAXHEAP
      && isHeap(pqueue)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for setPriority - leftist max to min
bool Tester::setPriorityFnNormalMaxLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, LEFTIST);
  cout << "Testing setPriorityFn- leftist heap -> max to min:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }
  
  // reset the heap type
  pqueue.setPriorityFn(priorityFn2, MINHEAP);

  // confirm that heap properties are maintained
  if (pqueue.getPriorityFn() == priorityFn2
      && pqueue.getHeapType() == MINHEAP
      && isHeap(pqueue)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for setPriority - skew min to max
bool Tester::setPriorityFnNormalMinSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, SKEW);
  cout << "Testing setPriorityFn- skew heap -> min to max:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // reset the heap type
  pqueue.setPriorityFn(priorityFn1, MAXHEAP);

  // confirm that heap properties are maintained
  if (pqueue.getPriorityFn() == priorityFn1
      && pqueue.getHeapType() == MAXHEAP
      && isHeap(pqueue)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for setPriority - skew max to min
bool Tester::setPriorityFnNormalMaxSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, SKEW);
  cout << "Testing setPriorityFn- skew heap -> max to min:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }

  // reset the heap type
  pqueue.setPriorityFn(priorityFn2, MINHEAP);

  // confirm that heap properties are maintained
  if (pqueue.getPriorityFn() == priorityFn2
      && pqueue.getHeapType() == MINHEAP
      && isHeap(pqueue)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for setStructure - min leftist to skew
bool Tester::setStructureNormalMinLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing setStructure- min heap -> leftist to skew:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // reset the heap type
  pqueue.setStructure(SKEW);

  // confirm that heap properties are maintained
  if (pqueue.getStructure() == SKEW
      && pqueue.getPriorityFn() == priorityFn2
      && pqueue.getHeapType() == MINHEAP
      && isHeap(pqueue)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for setStructure - max leftist to skew
bool Tester::setStructureNormalMaxLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, LEFTIST);
  cout << "Testing setStructure- max heap -> leftist to skew:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // reset the heap type
  pqueue.setStructure(SKEW);

  // confirm that heap properties are maintained
  if (pqueue.getStructure() == SKEW
      && pqueue.getPriorityFn() == priorityFn1
      && pqueue.getHeapType() == MAXHEAP
      && isHeap(pqueue)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for setStructure - min skew to leftist
bool Tester::setStructureNormalMinSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, SKEW);
  cout << "Testing setStructure- min heap -> skew to leftist:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // reset the heap type
  pqueue.setStructure(LEFTIST);

  // confirm that heap properties are maintained
  if (pqueue.getStructure() == LEFTIST
      && pqueue.getPriorityFn() == priorityFn2
      && pqueue.getHeapType() == MINHEAP
      && isHeap(pqueue)){
    return true;
  }
  else{
    return false;
  }
}

// test normal case for setStructure - max skew to leftist
bool Tester::setStructureNormalMaxSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, SKEW);
  cout << "Testing setStructure- max heap -> skew to leftist:\n";

  // generate a vector of random patients
  vector<Patient> patients;
  randPatients(NUM_PATIENTS, patients);

  // insert the patients into the heap
  for (auto it = patients.begin(); it < patients.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // reset the heap type
  pqueue.setStructure(LEFTIST);

  // confirm that heap properties are maintained
  if (pqueue.getStructure() == LEFTIST
      && pqueue.getPriorityFn() == priorityFn1
      && pqueue.getHeapType() == MAXHEAP
      && isHeap(pqueue)){
    return true;
  }
  else{
    return false;
  }
}

// test normal cases for assignment operator
bool Tester::assignNormalMinLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, LEFTIST);
  cout << "Testing assignment operator- min leftist:\n";
  vector<Patient> patients1;
  randPatients(NUM_PATIENTS, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // instantiate another heap
  vector<Patient> patients2;
  randPatients((NUM_PATIENTS / 2), patients2);
  PQueue pqueue1 = PQueue(priorityFn2, MINHEAP, LEFTIST);
  for (auto it = patients2.begin(); it < patients2.end(); ++it){
    pqueue1.insertPatient(*it);
  }
  // assign to another heap
  pqueue1 = pqueue;
  // confirm that heap properties are maintained
  if (pqueue1.getStructure() == LEFTIST
      && pqueue1.getPriorityFn() == priorityFn2
      && pqueue1.getHeapType() == MINHEAP
      && isHeap(pqueue1)){
    return true;
  }
  else{
    return false;
  }
}

// test normal cases for assignment operator
bool Tester::assignNormalMaxLeftist(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, LEFTIST);
  cout << "Testing assignment operator- max leftist:\n";
  vector<Patient> patients1;
  randPatients(NUM_PATIENTS, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // instantiate another heap
  vector<Patient> patients2;
  randPatients((NUM_PATIENTS / 2), patients2);
  PQueue pqueue1 = PQueue(priorityFn2, MINHEAP, LEFTIST);
  for (auto it = patients2.begin(); it < patients2.end(); ++it){
    pqueue1.insertPatient(*it);
  }
  // assign to another heap
  pqueue1 = pqueue;
  // confirm that heap properties are maintained
  if (pqueue1.getStructure() == LEFTIST
      && pqueue1.getPriorityFn() == priorityFn1
      && pqueue1.getHeapType() == MAXHEAP
      && isHeap(pqueue1)){
    return true;
  }
  else{
    return false;
  }
}

// test normal cases for assignment operator
bool Tester::assignNormalMinSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn2, MINHEAP, SKEW);
  cout << "Testing assignment operator- min skew:\n";
  vector<Patient> patients1;
  randPatients(NUM_PATIENTS, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // instantiate another heap
  vector<Patient> patients2;
  randPatients((NUM_PATIENTS / 2), patients2);
  PQueue pqueue1 = PQueue(priorityFn2, MINHEAP, LEFTIST);
  for (auto it = patients2.begin(); it < patients2.end(); ++it){
    pqueue1.insertPatient(*it);
  }
  // assign to another heap
  pqueue1 = pqueue;
  // confirm that heap properties are maintained
  if (pqueue1.getStructure() == SKEW
      && pqueue1.getPriorityFn() == priorityFn2
      && pqueue1.getHeapType() == MINHEAP
      && isHeap(pqueue1)){
    return true;
  }
  else{
    return false;
  }
}

// test normal cases for assignment operator
bool Tester::assignNormalMaxSkew(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, SKEW);
  cout << "Testing assignment operator- max skew:\n";
  vector<Patient> patients1;
  randPatients(NUM_PATIENTS, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // instantiate another heap
  vector<Patient> patients2;
  randPatients((NUM_PATIENTS / 2), patients2);
  PQueue pqueue1 = PQueue(priorityFn2, MINHEAP, LEFTIST);
  for (auto it = patients2.begin(); it < patients2.end(); ++it){
    pqueue1.insertPatient(*it);
  }
  // assign to another heap
  pqueue1 = pqueue;
  // confirm that heap properties are maintained
  if (pqueue1.getStructure() == SKEW
      && pqueue1.getPriorityFn() == priorityFn1
      && pqueue1.getHeapType() == MAXHEAP
      && isHeap(pqueue1)){
    return true;
  }
  else{
    return false;
  }
}

// test edge case for assignment operator
bool Tester::assignEdgeToEmpty(){
  // instantiate a heap
  PQueue pqueue = PQueue(priorityFn1, MAXHEAP, SKEW);
  cout << "Testing assignment operator- non-empty to empty:\n";
  vector<Patient> patients1;
  randPatients(NUM_PATIENTS, patients1);
  for (auto it = patients1.begin(); it < patients1.end(); ++it){
    pqueue.insertPatient(*it);
  }
  // instantiate another heap
  PQueue pqueue1 = PQueue(priorityFn1, MAXHEAP, SKEW);

  // assign to an empty heap
  pqueue = pqueue1;

  // pqueue should be empty
  if (pqueue.m_heap == nullptr){
    return true;
  }
  else{
    return false;
  }
}


// Display the result of the test
void Tester::resultMsg(string caseType, string methodName, string result){
  cout << "Test of " << caseType << " case for " << methodName
       << " " << result << ".\n";
}

// helper to generate a vector of patients
void Tester::randPatients(int numPatients, vector<Patient> &patients){
  vector<int> names;
  // have unique patient names, (SHUFFLE type)
  Random randNames = Random(1, numPatients, SHUFFLE);
  randNames.getShuffle(names);
  
  // all other params can have repeats
  Random temperature(MINTEMP, MAXTEMP);
  Random oxygen(MINOX, MAXOX);
  Random respiratory(MINRR, MAXRR);
  Random bloodPressure(MINBP, MAXBP);
  Random opinion(MINOPINION, MAXOPINION);

  // create and add patients to the vector
  for (int i = 0; i < numPatients; ++i){
    Patient patient("Patient #" + to_string(names.at(i)), temperature.getRandNum(),
		    oxygen.getRandNum(), respiratory.getRandNum(),
		    bloodPressure.getRandNum(), opinion.getRandNum());
    patients.push_back(patient);
  }
}

// check if the queue maintains heap properties
bool Tester::isHeap(const PQueue &queue){
  return isHeap(queue.m_heap, queue.m_heapType, queue.m_priorFunc);
}

// helper for checking if the queue maintains heap properties
bool Tester::isHeap(Node* node, const HEAPTYPE &heapType, const prifn_t &priorFunc){
  int nplLeft;
  int nplRight;
  bool isHeapProperty = true;

  // traverse until nullptr, or do not traverse if tree is empty
  if (node != nullptr){
    // traverse left
    if (node->m_left != nullptr){
      nplLeft = node->m_left->getNPL();
      // check min or max
      if (heapType == MINHEAP
	  && priorFunc(node->m_left->m_patient) < priorFunc(node->m_patient)){
        return false;
      }
      else if (heapType == MAXHEAP
	       && priorFunc(node->m_left->m_patient) > priorFunc(node->m_patient)){
        return false;
      }
      // continue traversing
      isHeapProperty = isHeap(node->m_left, heapType, priorFunc);
    }
    // otherwise, left child is nullptr, set values for compare with right
    else{
      nplLeft = 0;
    }
    // traverse right
    if (node->m_right != nullptr){
      nplRight = node->m_right->getNPL();
      // check min or max
      if (heapType == MINHEAP
          && priorFunc(node->m_right->m_patient) < priorFunc(node->m_patient)){
        return false;
      }
      else if (heapType == MAXHEAP
               && priorFunc(node->m_right->m_patient) > priorFunc(node->m_patient)){
        return false;
      }
      // continue traversing
      isHeapProperty = isHeap(node->m_right, heapType, priorFunc);
    }
    // otherwise, right child is nullptr, set values for compare with right
    else{
      nplRight = 0;
    }
    // check NPL difference for leftist
    if (nplLeft < nplRight){
      return false;
    }
  }
  // empty heap is balanced
  else{
    return true;
  }
  return isHeapProperty;
}

// check if two valid patients are equal
bool Tester::isEqualPatient(Patient patient1, Patient patient2){
  if (patient1.m_patient == patient2.m_patient
      && patient1.m_temperature == patient2.m_temperature
      && patient1.m_oxygen == patient2.m_oxygen
      && patient1.m_RR == patient2.m_RR
      && patient1.m_BP == patient2.m_BP
      && patient1.m_opinion == patient2.m_opinion){
    return true;
  }
  else{
    return false;
  }
}

// confirm correct NPL for a leftist heap
bool Tester::isNPL(const PQueue &queue){
  return isNPL(queue.m_heap);
}

bool Tester::isNPL(Node* node){
  if (node != nullptr){
    int count = 0;
    Node* temp = node;
    // count the number of right childre
    while (temp->m_right != nullptr){
      ++count;
      temp = temp->m_right;
    }
    
    if (node->getNPL() != count){
      return false;
    }
    // traverse left then right
    isNPL(node->m_left);
    isNPL(node->m_right);
  }
  return true;
}


// driver to test the class
int main(){
  // instantiate the Tester object
  Tester tester;
  string testCase;

  // Overloaded constructor - normal
  testCase = "Overloaded constructor";
  if (tester.constructNormal1()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // Overloaded constructor - normal
  if (tester.constructNormal2()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }
 
  // insertPatient(...) - normal
  testCase = "insertPatient(...)";
  if (tester.insertNormalMinLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // insertPatient(...) - normal
  if (tester.insertNormalMaxLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // insertPatient(...) - normal
  if (tester.insertNormalNPL()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }


  // insertPatient(...) - normal
  if (tester.insertNormalMinSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // insertPatient(...) - normal
  if (tester.insertNormalMaxSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // insertPatient(...) - error
  if (tester.insertErrorEmpty()){
    tester.resultMsg(ERROR, testCase, PASS);
  }
  else {
    tester.resultMsg(ERROR, testCase, FAIL);
  }

    // clear() - normal
  testCase = "clear()";
  if (tester.clearNormal()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // clear() - error
  if (tester.clearError()){
    tester.resultMsg(ERROR, testCase, PASS);
  }
  else {
    tester.resultMsg(ERROR, testCase, FAIL);
  }

  // getNextPatient(...) - normal
  testCase = "getNextPatient()";
  if (tester.removeNormalMinLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // getNextPatient(...) - normal
  if (tester.removeNormalMaxLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // getNextPatient(...) - normal
  if (tester.removeNormalMinSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // getNextPatient(...) - normal
  if (tester.removeNormalMaxSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // getNextPatient(...) - error
  if (tester.removeErrorEmpty()){
    tester.resultMsg(ERROR, testCase, PASS);
  }
  else {
    tester.resultMsg(ERROR, testCase, FAIL);
  }

  // mergeWithQueue(...) - normal
  testCase = "mergeWithQueue(...)";
  if (tester.mergeNormalMinLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // mergeWithQueue(...) - normal
  if (tester.mergeNormalMaxLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // mergeWithQueue(...) - normal
  if (tester.mergeNormalMinSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // mergeWithQueue(...) - normal
  if (tester.mergeNormalMaxSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // mergeWithQueue(...) - edge
  if (tester.mergeEdgeNonEmptytoEmpty()){
    tester.resultMsg(EDGE, testCase, PASS);
  }
  else {
    tester.resultMsg(EDGE, testCase, FAIL);
  }

  // mergeWithQueue(...) - edge
  if (tester.mergeEdgeEmptytoNonEmpty()){
    tester.resultMsg(EDGE, testCase, PASS);
  }
  else {
    tester.resultMsg(EDGE, testCase, FAIL);
  }

  // mergeWithQueue(...) - error
  if (tester.mergeErrorSelf()){
    tester.resultMsg(ERROR, testCase, PASS);
  }
  else {
    tester.resultMsg(ERROR, testCase, FAIL);
  }

  // mergeWithQueue(...) - error
  if (tester.mergeErrorDiffTypes()){
    tester.resultMsg(ERROR, testCase, PASS);
  }
  else {
    tester.resultMsg(ERROR, testCase, FAIL);
  }
  
  // copy constructor - normal
  testCase = "copy constructor";
  if (tester.copyNormalMinLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // copy constructor - normal
  if (tester.copyNormalMaxLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // copy constructor - normal
  if (tester.copyNormalMinSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }


  // copy constructor - normal
  if (tester.copyNormalMaxSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // copy constructor - edge
  if (tester.copyEdgeEmpty()){
    tester.resultMsg(EDGE, testCase, PASS);
  }
  else {
    tester.resultMsg(EDGE, testCase, FAIL);
  }

  // setPriorityFn(...) - normal
  testCase = "setPriorityFn(...)";
  if (tester.setPriorityFnNormalMinLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // setPriorityFn(...) - normal
  if (tester.setPriorityFnNormalMaxLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // setPriorityFn(...) - normal
  if (tester.setPriorityFnNormalMinSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // setPriorityFn(...) - normal
  if (tester.setPriorityFnNormalMaxSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // setStructure(...) - normal
  testCase = "setStructure(...)";
  if (tester.setStructureNormalMinLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // setStructure(...) - normal
  if (tester.setStructureNormalMaxLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // setStructure(...) - normal
  if (tester.setStructureNormalMinSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // setStructure(...) - normal
  if (tester.setStructureNormalMaxSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // assignment - normal
  testCase = "assignment operator";
  if (tester.assignNormalMinLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // assignment - normal
  if (tester.assignNormalMaxLeftist()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // assignment - normal
  if (tester.assignNormalMinSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // assignment - normal
  if (tester.assignNormalMaxSkew()){
    tester.resultMsg(NORM, testCase, PASS);
  }
  else {
    tester.resultMsg(NORM, testCase, FAIL);
  }

  // assignment - edge
  if (tester.assignEdgeToEmpty()){
    tester.resultMsg(EDGE, testCase, PASS);
  }
  else {
    tester.resultMsg(EDGE, testCase, FAIL);
  } 
  return 0;
}
