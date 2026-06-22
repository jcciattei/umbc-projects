// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"
// overloaded constructor
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure){
  // set properties and call relevant setters
  m_heap = nullptr;
  m_size = 0;
  setPriorityFn(priFn, heapType);
  setStructure(structure);
}

// destructor
PQueue::~PQueue(){
  clear();  
}

// reverts the object instance to empty
void PQueue::clear() {
  // if object is not empty, revert to empty
  if (m_heap != nullptr){
    clear(m_heap);
    m_heap = nullptr;
    m_size = 0;
  }
}

// copy constructor
PQueue::PQueue(const PQueue& rhs){
  m_heap = nullptr;
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;
  
  // traverse the right hand side and insert
  m_heap = copyQueue(m_heap, rhs.m_heap);
}

// overloaded assignment operator
PQueue& PQueue::operator=(const PQueue& rhs) {
  // protect against self assignment
  if (this != &rhs){
    clear();
    m_heap = nullptr;
    m_size = rhs.m_size;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;

    // traverse the right hand side and insert
    m_heap = copyQueue(m_heap, rhs.m_heap);
  }
  return *this;
}

// merges two distinct heaps of same type
// cannot self merge
// Throws domain_error if attempting invalid merge
void PQueue::mergeWithQueue(PQueue& rhs) {
  // protect against self merge
  if (this == &rhs){
    return;
  }
  // ensure compatible heaps before attempting merge
  else if (m_priorFunc == rhs.getPriorityFn()
	   && m_heapType == rhs.getHeapType()
	   && m_structure == rhs.getStructure()){

    m_size += rhs.m_size;
    m_heap = merge(m_heap, rhs.m_heap);
    // after merge, rhs is empty
    rhs.m_heap = nullptr;
    rhs.m_size = 0;
  }
  else{
    throw domain_error("Incompatible heaps- cannot merge.\n");
  }
}

// adds a patient to the queue/heap
void PQueue::insertPatient(const Patient& patient) {
  // check for empty patient
  if (patient == EMPTY){
    return;
  }
  // allocate the new Node
  Node* newNode = new Node(patient);
  
  // if heap is empty, make root
  if (m_heap == nullptr){
    m_heap = newNode;
    ++m_size;
  }
  // otherwise, create new subheap and
  // recursively merge to create new heap
  else{
    PQueue* newQueue = new PQueue(m_priorFunc, m_heapType, m_structure);
    newQueue->m_heap = newNode;
    newQueue->m_size += 1;
    mergeWithQueue(*newQueue);
    // after merge, subheap is empty
    delete newQueue;
  }
  // patient added to queue, m_size incremented in mergeWithQueue
}

// getter for the number of patients in the queue
int PQueue::numPatients() const {
  // number of patients is size of heap
  return m_size;
}

// getter for the priority function of the heap
prifn_t PQueue::getPriorityFn() const {
  return m_priorFunc;
}

// getter for the next patient (root node)
// Throw an out_of_range exception if queue is empty
Patient PQueue::getNextPatient() {
  // if heap is empty, throw exception
  if (m_heap == nullptr){
    throw out_of_range("Heap is empty- no patient to retrieve.\n");
  }
  // otherwise, remove root and merge remaining heaps
  else{
    Node *toDelete = m_heap;
    Patient patient = toDelete->m_patient;
    m_heap = merge(m_heap->m_left, m_heap->m_right);
    delete toDelete;
    return patient;
  }
  // patient is removed
  --m_size;
}

// setter for the priority function and heap type (min or max)
// must rebuild heap using current data
void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  m_priorFunc = priFn;
  m_heapType = heapType;

  // a non-empty heap must be rebuilt
  if (m_heap != nullptr){
    // create a temp copy
    PQueue* temp = new PQueue(*this);
    Patient patient;
    
    // remove all patients from queue
    // note that clear does not change m_priorFunc, m_heapType, or m_structure
    clear();
    
    while (temp->m_heap != nullptr){
      patient = temp->getNextPatient();
      insertPatient(patient);
    }
    delete temp;
  }
}

// setter for the type of structure (leftist or skew)
// must rebuild heap utilizing current data
void PQueue::setStructure(STRUCTURE structure){
  m_structure = structure;

  // a non-empty heap must be rebuilt
  if (m_heap != nullptr){
    // create a temp copy
    PQueue* temp = new PQueue(*this);
    Patient patient;

    // remove all patients from queue
    // note that clear does not change m_priorFunc, m_heapType, or m_structure
    clear();
   
    while (temp->m_heap != nullptr){
      patient = temp->getNextPatient();
      insertPatient(patient);
    }
    delete temp;
  }
}

// getter for heap type (min or max)
HEAPTYPE PQueue::getHeapType() const {
  return m_heapType;
}

// getter for the type of structure (leftist or skew)
STRUCTURE PQueue::getStructure() const {
  return m_structure;
}

// print the queue using preorder traversal
// root, left, right. Priority of root is highest
void PQueue::printPatientQueue() const {
  cout << "Contents of the queue:\n";
  if (m_heap != nullptr){
    printPatientQueue(m_heap);
  }
}

// provided for debugging ////////////////////////////////////////////////
void PQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}
void PQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
    else
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
  sout  << patient.getPatient() 
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}

/////////////////////////////////////////////////////////////////////////////

// helpers

// helper for recursive traversal when clearing the heap
void PQueue::clear(Node *node){
  if (node != nullptr){
    // traverse left then right
    clear(node->m_left);
    clear(node->m_right);
    // zero out the object and deallocate
    node->getPatient().setPatient("");
    node->getPatient().setTemperature(0);
    node->getPatient().setOxygen(0);
    node->getPatient().setRR(0);
    node->getPatient().setBP(0);
    node->getPatient().setOpinion(0);
    node->setNPL(0);
    // left and right children are nullptr
    delete node;
    node = nullptr;
  }
}

// helper for recursive traversal while merging
// precondition- heaps must be of same type and structure
// and have same priority function
Node* PQueue::merge(Node* heapRoot1, Node* heapRoot2){
  // if one heap is empty, return the other
  if (heapRoot1 == nullptr){
    return heapRoot2;
  }
  if (heapRoot2 == nullptr){
    return heapRoot1;
  }
  // depending upon min or max heap, determine how to merge
  // assume heapRoot1 has higher priority
  // if min, and heapRoot2 is higher priority, merge 1 to 2 
  if (m_heapType == MINHEAP
      && ((*m_priorFunc)(heapRoot2->getPatient())
	  < (*m_priorFunc)(heapRoot1->getPatient()))){
    return mergeSwap(heapRoot2, heapRoot1);
  }
  // if max, and heapRoot 2 has higher priority, merge 1 to 2
  else if (m_heapType == MAXHEAP
      && ((*m_priorFunc)(heapRoot2->getPatient())
          > (*m_priorFunc)(heapRoot1->getPatient()))){
    return mergeSwap(heapRoot2, heapRoot1);
  }
  // otherwise if max or min, merge 2 to 1
  else{
    return mergeSwap(heapRoot1, heapRoot2);
  }
}

// secondary helper for merging
Node* PQueue::mergeSwap(Node* heapRoot1, Node* heapRoot2){
  // if left child of host heap is nullptr, set merging
  // heap to left child
  if (heapRoot1->m_left == nullptr){
    heapRoot1->m_left = heapRoot2;
  }
  // otherwise, recursive merge
  else{
    heapRoot1->m_right = merge(heapRoot1->m_right, heapRoot2);

    // for a skew heap, always swap children
    // for a leftist heap, swap children if the left npl is less
    // than the right npl
    if (m_structure == SKEW ||
	(heapRoot1->m_left->getNPL() < heapRoot1->m_right->getNPL())){

      swapChildren(heapRoot1);
    }
    // update the null path length if leftist
    if (m_structure == LEFTIST){
      heapRoot1->setNPL(heapRoot1->m_right->getNPL() + 1);
    }
  }
  return heapRoot1;
}

// swap children of a node
void PQueue::swapChildren(Node* node){
  Node* temp = node->m_left;
  node->m_left = node->m_right;
  node->m_right = temp;
}

// copy a queue (from a Node pointer)
Node* PQueue::copyQueue(Node* node1, const Node *node2){
  Node * newNode;
  if (node2 != nullptr){
    // copy data from source queue
    newNode = new Node(node2->m_patient);
    node1 = newNode;
    node1->setNPL(node2->getNPL());
    // traverse left
    node1->m_left = copyQueue(node1->m_left, node2->m_left);
    // traverse right
    node1->m_right = copyQueue(node1->m_right, node2->m_right);
  }
  return node1;
}

// helper for recersive traversal while printing
// preorder traversal > root, left, right
void PQueue::printPatientQueue(Node* node) const{
  if (node != nullptr){
    cout << "[" << (*m_priorFunc)(node->getPatient()) << "] "
	 << node->m_patient << "\n";
    printPatientQueue(node->m_left);
    printPatientQueue(node->m_right);
  }
}
