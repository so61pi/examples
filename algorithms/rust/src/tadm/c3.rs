// search(D, value)
// insert(D, value)
// delete(D, value)
// min(D)
// max(D)
// successor(D, node)
// predecessor(D, node)

pub fn fn_01(s: &str) -> Option<usize> {
    // [3] A common problem for compilers and text editors is determining
    // whether the parentheses in a string are balanced and properly nested. For
    // example, the string ((())())() contains properly nested pairs of
    // parentheses, which the strings )()( and ()) do not. Give an algorithm
    // that returns true if a string contains properly nested and balanced
    // parentheses, and false if otherwise. For full credit, identify the
    // position of the first offending parenthesis if the string is not properly
    // nested and balanced.

    let iter = s.chars().map(|c| match c {
        '(' => 1,
        ')' => -1,
        _ => panic!("invalid character"),
    });
    let mut first_unclosed = 0;
    let mut sum = 0;
    for (i, e) in iter.enumerate() {
        assert!(-1 <= sum);

        let newsum = sum + e;
        assert!(-1 <= newsum);

        // Wrong ')'.
        if newsum == -1 {
            return Some(i);
        }

        // Record potentially wrong '('.
        if sum == 0 && newsum == 1 {
            first_unclosed = i;
        }

        sum = newsum;
    }

    assert!(sum >= 0);
    if sum > 0 {
        return Some(first_unclosed);
    }

    None
}

// TODO: Skip
pub fn fn_02() {
    // [3] Write a program to reverse the direction of a given singly-linked
    // list. In other words, after the reversal all pointers should now point
    // backwards. Your algorithm should take linear time.
}

// TODO: Skip
pub fn fn_03() {
    // [5] We have seen how dynamic arrays enable arrays to grow while still
    // achieving constant-time amortized performance. This problem concerns
    // extending dynamic arrays to let them both grow and shrink on demand.
    //
    // (a) Consider an underflow strategy that cuts the array size in half
    // whenever the array falls below half full. Give an example sequence of
    // insertions and deletions where this strategy gives a bad amortized cost.
    //
    // (b) Then, give a better underflow strategy than that suggested above, one
    // that achieves constant amortized cost per deletion.
}

pub fn fn_04() {
    // [3] Design a dictionary data structure in which search, insertion, and
    // deletion can all be processed in O(1) time in the worst case. You may
    // assume the set elements are integers drawn from a finite set 1, 2, .., n,
    // and initialization can take O(n) time.

    // Assumes that the elements are N unique numbers, we can initialize a hash
    // table with N buckets and each element will go to separated bucket.
}

// TODO: Skip
pub fn fn_05() {
    // [3] Find the overhead fraction (the ratio of data space over total space)
    // for each of the following binary tree implementations on n nodes:
    //
    // (a) All nodes store data, two child pointers, and a parent pointer. The
    // data field requires four bytes and each pointer requires four bytes.
    //
    // (b) Only leaf nodes store data; internal nodes store two child pointers.
    // The data field requires four bytes and each pointer requires two bytes.
}

pub fn fn_06() {
    // [5] Describe how to modify any balanced tree data structure such that
    // search, insert, delete, minimum, and maximum still take O(log n) time
    // each, but successor and predecessor now take O(1) time each. Which
    // operations have to be modified to support this?

    // Add additional pointers for successor & predecessor and keep them in sync
    // when insert or delete.
}

pub fn fn_07() {
    // [5] Suppose you have access to a balanced dictionary data structure,
    // which supports each of the operations search, insert, delete, minimum,
    // maximum, successor, and predecessor in O(log n) time. Explain how to
    // modify the insert and delete operations so they still take O(log n) but
    // now minimum and maximum take O(1) time. (Hint: think in terms of using
    // the abstract dictionary operations, instead of mucking about with
    // pointers and the like.)

    // Place additional variables to hold min and max values out side of the
    // tree and keep them in sync when insert or delete.
    //
    // new_insert(e) {
    //     insert(e)
    //     min, max = minmax(min, e, max)
    // }
    //
    // new_delete(e) {
    //     delete(e)
    //     if e == min {
    //         min = successor(e)
    //     }
    //     if e == max {
    //         max = predecessor(e)
    //     }
    // }
}

pub fn fn_08() {
    // [6] Design a data structure to support the following operations:
    //
    // • insert(x,T) – Insert item x into the set T .
    //
    // • delete(k,T) – Delete the kth smallest element from T .
    //
    // • member(x,T) – Return true iff x ∈ T .
    //
    // All operations must take O(log n) time on an n-element set.

    // Binary search tree with each node contains the number of left and right
    // child elements.
    //
    // insert(x,T) increases children number from root to element.
    //
    // delete(k,T) decreases children number from root to the largest deleted
    // element.
    //
    // member(x, T) is search.
}

// TODO
pub fn fn_09() {
    // [8] A concatenate operation takes two sets S1 and S2 , where every key in
    // S1 is smaller than any key in S2 , and merges them together. Give an
    // algorithm to concatenate two binary search trees into one binary search
    // tree. The worst-case running time should be O(h), where h is the maximal
    // height of the two trees.
}

// TODO
pub fn fn_10() {
    // [5] In the bin-packing problem, we are given n metal objects, each
    // weighing between zero and one kilogram. Our goal is to find the smallest
    // number of bins that will hold the n objects, with each bin holding one
    // kilogram at most.
    //
    // • The best-fit heuristic for bin packing is as follows. Consider the
    // objects in the order in which they are given. For each object, place it
    // into the partially filled bin with the smallest amount of extra room
    // after the object is inserted.. If no such bin exists, start a new bin.
    // Design an algorithm that implements the best-fit heuristic (taking as
    // input the n weights w1 , w2, ..., wn and outputting the number of bins
    // used) in O(n log n) time.
    //
    // • Repeat the above using the worst-fit heuristic, where we put the next
    // object in the partially filled bin with the largest amount of extra room
    // after the object is inserted.
}

pub fn fn_11() {
    // [5] Suppose that we are given a sequence of n values x1 , x2 , ..., xn
    // and seek to quickly answer repeated queries of the form: given i and j,
    // find the smallest value in xi , . . . , xj .
    //
    // (a) Design a data structure that uses O(n2 ) space and answers queries in
    // O(1) time.
    //
    // (b) Design a data structure that uses O(n) space and answers queries in
    // O(log n) time. For partial credit, your data structure can use O(n log n)
    // space and have O(log n) query time.

    // (a) A square matrix that contains smallest values of any given pair of
    // (i, j).
    /*
       i
       |
       v
    j->  0 1 2 ... n
       0
       1
       2
       .
       n
    */
    // (b) Have a binary tree that each node contains
    // - min value of its child nodes
    // - index of that min value
    // - index range that the min value covers
    // Note: min(a0, a1, a2, a3) is a2 => min(a1, a2) is a2
    /*
                  0
                 [0]
                 0:7
           _____/   \_____
          0               8
         [0]             [4]
         0:3             4:7
       _/   \_         _/   \_
      0       4       8       C
     [0]     [2]     [4]     [6]
     0:1     2:3     4:5     6:7
     / \     / \     / \     / \
    0   2   4   6   8   A   C   E
   [0] [1] [2] [3] [4] [5] [6] [7]
    :   :   :   :   :   :   :   :
    */
}

pub fn fn_12() {
    // [5] Suppose you are given an input set S of n numbers, and a black box
    // that if given any sequence of real numbers and an integer k instantly and
    // correctly answers whether there is a subset of input sequence whose sum
    // is exactly k. Show how to use the black box O(n) times to find a subset
    // of S that adds up to k.

    // Put everything to the box, then remove element one by one. If the box
    // returns no for an element, don't remove that one. At the end, we will
    // have a set of elements that adds up the k since the box returns yes for
    // that set and returns no when any element of that set is removed.
}

pub fn fn_13() {
    // [5] Let A[1..n] be an array of real numbers. Design an algorithm to
    // perform any sequence of the following operations:
    //
    // • Add(i,y) – Add the value y to the ith number.
    //
    // • Partial-sum(i) – Return the sum of the first i numbers, i.e. SUM(j=1 ->
    //  i, A[j]).
    //
    // There are no insertions or deletions; the only change is to the values of
    // the numbers. Each operation should take O(log n) steps. You may use one
    // additional array of size n as a work space.

    // O(log n) so we need a tree-like data structure. As we are only allowed to
    // use an array of N elements at max, a binary heap is our choice.
    //
    // Our binary heap is organized like below. Each node contains the sum, the
    // index range of all child nodes.
    /*
         a   b   c   d   e   f
         .       .       .   .
        L R     L R     L R   R
       /   \   /   \   /   \   \
     +---+---+---+---+---+---+---+
     | 5 | 8 | 3 | 6 | 7 | 2 | 4 |
     +---+---+---+---+---+---+---+
       0   1   2   3   4   5   6

                    d
                  [0:6]
                   35
            .-------^-------.
            b               f
          [0:3]           [4:6]
           22              13 (+4)
        .---^---.       .---^
        a       c       e
      [0:1]   [2:3]   [4:5]
       13       9       9
      (5+8)   (3+6)   (7+2)

    Node {
        l_index:r_index
        sum_of_children

        l_node
        r_node
    }
    */
    // For function Add, after adding value to the corresponding element in
    // array, we find the leaf node for that index and add the provided value to
    // node.sum for found node and all of its parents.
    //
    // PartialSum can be implemented by splitting the given index in two index
    // ranges then search for the their values. For instance, suppose we compute
    // PartialSum(5), then we split 5 into [0:3] and [4:5] (for left and right
    // branches) and then start searching. We can also implement this through
    // subtraction like [0:6] - array[6].
}

// TODO
pub fn fn_14() {
    // [8] Extend the data structure of the previous problem to support
    // insertions and deletions. Each element now has both a key and a value. An
    // element is accessed by its key. The addition operation is applied to the
    // values, but the elements are accessed by its key. The Partial sum
    // operation is different.
    //
    // • Add(k,y) – Add the value y to the item with key k.
    //
    // • Insert(k,y) – Insert a new item with key k and value y.
    //
    // • Delete(k) – Delete the item with key k.
    //
    // • Partial-sum(k) – Return the sum of all the elements currently in the
    // set whose key is less than y, i.e. SUM(xj < y, xi)
    //
    // The worst case running time should still be O(n log n) for any sequence
    // of O(n) operations.
}

// TODO
pub fn fn_15() {
    // [8] Design a data structure that allows one to search, insert, and delete
    // an integer X in O(1) time (i.e. , constant time, independent of the total
    // number of integers stored). Assume that 1 ≤ X ≤ n and that there are m +
    // n units of space available, where m is the maximum number of integers
    // that can be in the table at any one time. (Hint: use two arrays A[1..n]
    // and B[1..m].) You are not allowed to initialize either A or B, as that
    // would take O(m) or O(n) operations. This means the arrays are full of
    // random garbage to begin with, so you must be very careful.
}

// TODO: Skip
pub fn fn_16() {
    // [5] Implement versions of several different dictionary data structures,
    // such as linked lists, binary trees, balanced binary search trees, and
    // hash tables. Conduct exper- iments to assess the relative performance of
    // these data structures in a simple ap- plication that reads a large text
    // file and reports exactly one instance of each word that appears within
    // it. This application can be efficiently implemented by main- taining a
    // dictionary of all distinct words that have appeared thus far in the text
    // and inserting/reporting each word that is not found. Write a brief report
    // with your conclusions.
}

// TODO: Skip
pub fn fn_17() {
    // [5] A Caesar shift (see Section 18.6 (page 641)) is a very simple class
    // of ciphers for secret messages. Unfortunately, they can be broken using
    // statistical properties of English. Develop a program capable of
    // decrypting Caesar shifts of sufficiently long texts.
}

pub fn fn_18() {
    // [3] What method would you use to look up a word in a dictionary?

    // As words in dictionary are alphabetically sorted, we can use binary
    // search.
}

pub fn fn_19() {
    // [3] Imagine you have a closet full of shirts. What can you do to organize
    // your shirts for easy retrieval?

    // Array.
}

pub fn fn_20() {
    // [4] Write a function to find the middle node of a singly-linked list.

    // Pointer racing.
    /*
    Node {
        value
        next *Node
    }

    middle *Node = nullptr
    node *Node = init
    for i = 0; node != nullptr; ++i {
        if i % 2 == 0 {
            middle = middle->next
        }
        node = node->next
    }
    */
}

pub fn fn_21() {
    // [4] Write a function to compare whether two binary trees are identical.
    // Identical trees have the same key value at each position and the same
    // structure.

    // Basically do tree traversal for 2 trees at the same time.
}

pub fn fn_22() {
    // [4] Write a program to convert a binary search tree into a linked list.

    /*
    TreeToLList(tnode, llist) {
        if tnode->left {
            TreeToLList(tnode->left, llist)
        }

        Add(llist, tnode->value)

        if tnode->right {
            TreeToLList(tnode->right, llist)
        }
    }
    */
}

pub fn fn_23() {
    // [4] Implement an algorithm to reverse a linked list. Now do it without
    // recursion.

    /*
    newlist
    for node = list.pop_front_node(); node != null; node = list.pop_front_node() {
        newlist.push_front_node(node)
    }
    list = newlist

    pop_front_node() {
        if head == nullptr return nullptr
        node, head = head, head->next
        node->next = nullptr
        return node
    }

    push_front_node(node) {
        node->next = head
        head = node
    }
    */

    /*
    new *Node = nullptr
    for node = old, save = nullptr; node != nullptr; node = save {
        save = node->next

        node->next = new
        new = node
    }
    */
}

pub fn fn_24() {
    // [5] What is the best data structure for maintaining URLs that have been
    // visited by a Web crawler? Give an algorithm to test whether a given URL
    // has already been visited, optimizing both space and time.

    // We can use a tree (below), each node of it holds a part of URL. To
    // achieve more space efficiency, Bloom filter on hash of strings is
    // a suitable choice but we need to take care of saturation problem.
    /*
    http://example.com/page-1
    http://example.com/page-2

                "http:"
                |
                ""
                |
                "example.com"
                |           |
         "page-1"           "page-2"
    */
}

pub fn fn_25(search: &str, magazine: &str) -> bool {
    // [4] You are given a search string and a magazine. You seek to generate
    // all the characters in search string by cutting them out from the
    // magazine. Give an algorithm to efficiently determine whether the magazine
    // contains all the letters in the search string.

    // We can put the search string into an array. The array may contain empty
    // slots for missing letters. We also need a counter variable to keep track
    // of the number of letters that are already encountered.
    //
    // We then go the magazine letter by letter and check if it is in the array.
    // If it is, then we update the counter accordingly.

    let mut counter: usize = 0;
    let mut array = [None as Option<bool>; 256];
    for ch in search.bytes() {
        if let x @ None = &mut array[ch as usize] {
            x.replace(false);
            counter += 1;
        }
    }

    for ch in magazine.bytes() {
        if counter == 0 {
            return true;
        }

        if let Some(x @ false) = array[ch as usize].as_mut() {
            counter -= 1;
            *x = true;
        }
    }

    false
}

pub fn fn_26() {
    // [4] Reverse the words in a sentence—i.e., “My name is Chris” becomes
    // “Chris is name My.” Optimize for time and space.

    // Do following steps inplace in whatever order
    // - Reverse whole string.
    // - Reverse each word.
}

pub fn fn_27() {
    // [5] Determine whether a linked list contains a loop as quickly as
    // possible without using any extra storage. Also, identify the location of
    // the loop.

    // We can have at most one loop.
    //
    // Assume that the linked list contains the number of nodes, we can loop to
    // the end of list then check if next is actually null. If it is, then there
    // is no loop. Otherwise next points to the starting point of the cycle.
    //
    // If we don't have list length, we can detect loop using pointer racing. We
    // have a slow pointer that jumps 1 step and a fast pointer that jumps 2.
    // While fast is not null, if it is equal to slow then we have a loop.
}

pub fn fn_28(data: &[i64]) -> Vec<i64> {
    // [5] You have an unordered array X of n integers. Find the array M
    // containing n elements where Mi is the product of all integers in X except
    // for Xi . You may not use division. You can use extra memory. (Hint: There
    // are solutions faster than O(n2 ).)

    fn accproduct<'a>(it: impl Iterator<Item = &'a i64>) -> Vec<i64> {
        let mut v = it.fold([1i64].to_vec(), |mut acc, e| {
            acc.push(acc.last().unwrap() * e);
            acc
        });
        v.push(1i64);
        v
    }

    let lr_product = accproduct(data.iter());
    let mut rl_product = accproduct(data.iter().rev());
    rl_product.reverse();

    (1..=data.len()).fold(Vec::new(), |mut acc, e| {
        acc.push(lr_product.get(e - 1).unwrap() * rl_product.get(e + 1).unwrap());
        acc
    })
}

pub fn fn_29(text: &str) -> String {
    // [6] Give an algorithm for finding an ordered word pair (e.g., “New York”)
    // occurring with the greatest frequency in a given webpage. Which data
    // structures would you use? Optimize both time and space.

    // Using hashtable, each word pair goes to a different slot. Update the
    // highest frequency pair for every iteration.

    let mut it = text.split_whitespace();
    let first = if let Some(word) = it.next() {
        word
    } else {
        return String::new();
    };

    struct Max {
        pair: String,
        counter: u32,
    }

    impl Max {
        fn new() -> Max {
            Max {
                pair: String::new(),
                counter: 0,
            }
        }
    }

    let max = it
        .scan(first, |first, word| {
            let pair = format!("{} {}", first, word);
            *first = word;
            Some(pair)
        })
        .scan(std::collections::HashMap::new(), |st, pair| {
            let counter = st.entry(pair.clone()).or_insert(0);
            *counter += 1;
            Some((*counter, pair))
        })
        .fold(Max::new(), |max, (counter, pair)| {
            if counter > max.counter {
                Max { pair, counter }
            } else {
                max
            }
        });

    max.pair
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_fn_01() {
        assert!(fn_01("()").is_none());
        assert!(fn_01("()()").is_none());
        assert_eq!(fn_01("()(").unwrap(), 2);
        assert_eq!(fn_01("((").unwrap(), 0);
        assert_eq!(fn_01("(()").unwrap(), 0);
        assert_eq!(fn_01(")(()").unwrap(), 0);
        assert_eq!(fn_01("(").unwrap(), 0);
        assert_eq!(fn_01(")").unwrap(), 0);
    }

    #[test]
    fn test_fn_28() {
        assert_eq!(fn_28(&[1, 2, 3]), &[6, 3, 2]);
        assert_eq!(fn_28(&[1, 1, 1]), &[1, 1, 1]);
        assert_eq!(fn_28(&[1, 2, 1]), &[2, 1, 2]);
    }

    #[test]
    fn test_fn_29() {
        assert_eq!(fn_29(""), "");
        assert_eq!(fn_29("a"), "");
        assert_eq!(fn_29("a b"), "a b");
        assert_eq!(fn_29("a b b"), "a b");
        assert_eq!(fn_29("a b b b"), "b b");
        assert_eq!(fn_29("a b a b"), "a b");
    }
}
