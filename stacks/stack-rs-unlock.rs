use std::io;
use std::thread;
use std::sync::{Arc, Mutex};

// Stack struct
pub struct Stack {
  head: Option<Box<Node>>,
  len: u32
}

// Node structure for stack (linked list)
struct Node {
  key: i32,
  next: Option<Box<Node>>
}

// Implement Node type
impl Node {
  // new()
  // @params: (i32) key - key of new node
  // @return: (Self) - newly created node
  pub fn new(key: i32) -> Self {
    Node {
      key,
      next: None
    }
  }
}

// Implement Stack type
impl Stack {
  // new()
  // @params: (None)
  // @return: (Self) - newly created stack
  pub fn new() -> Self {
    Stack {
      head: None,
      len: 0
    }
  }

  // push()
  // @params: (mut self) - stack to be added to
  //          (i32) - key of node to be pushed
  // @return  (Self) - newly modified stack
  pub fn push(mut self, key: i32) -> Self {
    let mut new_node = Node::new(key);

    self.len += 1;
    new_node.next = self.head;
    self.head = Some(Box::new(new_node));
    return self;
  }
}

fn main() {
  let my_stack = Arc::new(Mutex::new(Stack::new()));
  let mut i: i32 = 0;
  let mut handles = vec![];

  while i < 100 {
    let my_stack = Arc::clone(&my_stack);
    let handle = thread::spawn(move || {
      let mut stack = my_stack.lock().unwrap();

      *stack.push(i.clone());
    });

    i += 1;
    handles.push(handle);
  }

  for handle in handles {
    handle.join().unwrap();
  }
}

