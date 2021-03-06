// singly linked list

#include <iostream>

using namespace std;


struct ListNode {
  int val;
  ListNode* next;
  ListNode(int _val=0) : val(_val), next(nullptr) {  }
};


class SinglyLinkedList {
 private:
  int m_size;
  ListNode* m_head;
  ListNode* m_tail;

 public:
  SinglyLinkedList() {
    m_size = 0;
    m_head = nullptr;
    m_tail = nullptr;
    cout << "Constructor is called." << endl;
  }

  void insertFront(int val) {
    if (m_size == 0) {
      m_head = m_tail = new ListNode(val);
      cout << "Insert a ListNode at front of an empty SinglyLinkedList...\n";
    } else {
      ListNode* ptr = new ListNode(val);
      ptr->next = m_head;
      m_head = ptr;
      cout << "Insert a ListNode at front of a non-empty SinglyLinkedList...\n";
    }
    ++m_size;
  }

  void insertBack(int val) {
    if (m_size == 0) {
      m_head = m_tail = new ListNode(val);
      cout << "Insert a ListNode at back of an empty SinglyLinkedList...\n";
    } else {
      m_tail->next = new ListNode(val);
      m_tail = m_tail->next;
      cout << "Insert a ListNode at back of a non-empty SinglyLinkedList...\n";
    }
    ++m_size;
  }

  void insertNode(int pos, int val) {
    if (pos <= 0) {
      cout << "Insert at front, call insertFront() ==>" << endl;
      insertFront(val);
    } else if (pos >= m_size) {
      cout << "Insert at back, call insertBack() ==>" << endl;
      insertBack(val);
    } else {
      ListNode *ptr1, *ptr2;
      ptr1 = m_head;
      for (int i = 0; i < pos - 1; ++i) {
        ptr1 = ptr1->next;
      }
      ptr2 = new ListNode(val);
      ptr2->next = ptr1->next;
      ptr1->next = ptr2;
      ++m_size;
      cout << "Insert at position pos of a SinglyLinkedList..." << endl;
    }
  }

  void deleteNode(int pos) {
    if (pos < 0 || pos > m_size - 1) {
      cout << "No ListNode to delete." << endl;
      return;
    }
    ListNode *ptr1, *ptr2;
    if (pos == 0) {
      ptr1 = m_head;
      if (m_size == 1) {
        m_head = m_tail = nullptr;
        cout << "Delete the first sole ListNode..." << endl;
      } else {
        m_head = m_head->next;
        cout << "Delete the first ListNode from a multiple-node SinglyLinkedList...\n";
      }
      delete ptr1;
    } else {
      ptr1 = m_head;
      for (int i = 0; i < pos - 1; ++i) {
        ptr1 = ptr1->next;
      }
      ptr2 = ptr1->next;
      ptr1->next = ptr2->next;
      if (ptr2->next == nullptr) {
        m_tail = ptr1;
        cout << "Delete the last ListNode from a multiple-node SinglyLinkedList...\n";
      }
      cout << "Delete a ListNode at postion pos of a SinglyLinkedList...\n";
      delete ptr2;
    }
    --m_size;
  }

  void updateNode(int pos, int val) {
    if (pos < 0 || pos > m_size - 1) {
      cout << "No ListNode for update." << endl;
      return;
    }
    ListNode* ptr = m_head;
    for (int i = 0; i < pos; ++i) {
      ptr = ptr->next;
    }
    ptr->val = val;
    cout << "Update a ListNode at position pos of a SinglyLinkedList...\n";
  }

  ListNode* findNode(int val) {
    ListNode* ptr = m_head;
    while (ptr != nullptr) {
      if (ptr->val == val) {
        cout << "ListNode is found." << endl;
        return ptr;
      }
      ptr = ptr->next;
    }
    cout << "ListNode not found." << endl;
    return nullptr;
  }

  ListNode* getNode(int pos) {
    if (pos < 0 || pos > m_size -1) {
      cout << "Invalid position." << endl;
      return nullptr;
    } else {
      ListNode* ptr = m_head;
      for (int i = 0; i < pos; ++i) {
        ptr = ptr->next;
      }
      cout << "ListNode is got." << endl;
      return ptr;
    }
  }

  ~SinglyLinkedList() {
    ListNode* ptr = m_head;
    while (m_head != nullptr) {
      m_head = m_head->next;
      delete ptr;
      ptr = m_head;
    }
    m_head = m_tail = nullptr;
    cout << "Destructor is called." << endl;
  }

  void output() {
    ListNode* ptr = m_head;
    while (ptr != nullptr) {
      cout << ptr->val << ", ";
      ptr = ptr->next;
    }
    cout << endl;
  }

  int getSize() {
    return m_size;
  }
};


int main(int argc, char* argv[]) {

  cout << "------------------------\n";
  SinglyLinkedList* lst1 = new SinglyLinkedList();
  lst1->insertFront(234);
  lst1->deleteNode(0);
  cout << lst1->getSize() << endl;
  delete lst1;

  cout << "------------------------\n";
  SinglyLinkedList* lst2 = new SinglyLinkedList();
  lst2->insertFront(234);
  lst2->insertBack(33);
  lst2->deleteNode(0);
  cout << lst2->getSize() << endl;
  delete lst2;

  cout << "------------------------\n";
  SinglyLinkedList* lst3 = new SinglyLinkedList();
  lst3->insertFront(10);
  lst3->insertBack(20);
  lst3->insertNode(2, -30);
  lst3->insertNode(2, 40);
  lst3->insertNode(0, 50);
  lst3->insertNode(2, 60);

  lst3->findNode(50);
  cout << lst3->getSize() << endl;
  lst3->output();
  lst3->updateNode(-1, 100);
  lst3->updateNode(6, 600);
  lst3->updateNode(0, 123);
  lst3->findNode(50);
  lst3->findNode(123);
  lst3->output();
  cout << lst3->getNode(0)->val << endl;
  delete lst3;

  return 0;
}


