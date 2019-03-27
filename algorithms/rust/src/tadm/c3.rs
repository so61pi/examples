// search(D, value)
// insert(D, value)
// delete(D, value)
// min(D)
// max(D)
// successor(D, node)
// predecessor(D, node)

pub fn fn_01(s: &str) -> Option<usize> {
    // A common problem for compilers and text editors is determining whether
    // the parentheses in a string are balanced and properly nested. For
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

// TODO
pub fn fn_02() {
    // Write a program to reverse the direction of a given singly-linked list.
    // In other words, after the reversal all pointers should now point
    // backwards. Your algorithm should take linear time.
}

// TODO
pub fn fn_03() {
    // We have seen how dynamic arrays enable arrays to grow while still
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
    // Design a dictionary data structure in which search, insertion, and
    // deletion can all be processed in O(1) time in the worst case. You may
    // assume the set elements are integers drawn from a finite set 1, 2, .., n,
    // and initialization can take O(n) time.

    // Assumes that the elements are N unique numbers, we can initialize a hash
    // table with N buckets and each element will go to separated bucket.
}

// TODO
pub fn fn_05() {
    // Find the overhead fraction (the ratio of data space over total space) for
    // each of the following binary tree implementations on n nodes:
    //
    // (a) All nodes store data, two child pointers, and a parent pointer. The
    // data field requires four bytes and each pointer requires four bytes.
    //
    // (b) Only leaf nodes store data; internal nodes store two child pointers.
    // The data field requires four bytes and each pointer requires two bytes.
}

pub fn fn_06() {
    // Describe how to modify any balanced tree data structure such that search,
    // insert, delete, minimum, and maximum still take O(log n) time each, but
    // successor and predecessor now take O(1) time each. Which operations have
    // to be modified to support this?

    // Add additional pointers for successor & predecessor and keep them in sync
    // when insert or delete.
}

pub fn fn_07() {
    // Suppose you have access to a balanced dictionary data structure, which
    // supports each of the operations search, insert, delete, minimum, maximum,
    // successor, and predecessor in O(log n) time. Explain how to modify the
    // insert and delete operations so they still take O(log n) but now minimum
    // and maximum take O(1) time. (Hint: think in terms of using the abstract
    // dictionary operations, instead of mucking about with pointers and the
    // like.)

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
    // Design a data structure to support the following operations:
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
    // A concatenate operation takes two sets S1 and S2 , where every key in S1
    // is smaller than any key in S2 , and merges them together. Give an
    // algorithm to concatenate two binary search trees into one binary search
    // tree. The worst-case running time should be O(h), where h is the maximal
    // height of the two trees.
}

// TODO
pub fn fn_10() {
    // In the bin-packing problem, we are given n metal objects, each weighing
    // between zero and one kilogram. Our goal is to find the smallest number of
    // bins that will hold the n objects, with each bin holding one kilogram at
    // most.
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

// TODO
pub fn fn_11() {
    // Suppose that we are given a sequence of n values x1 , x2 , ..., xn and
    // seek to quickly answer repeated queries of the form: given i and j, find
    // the smallest value in xi , . . . , xj .
    //
    // (a) Design a data structure that uses O(n2 ) space and answers queries in
    // O(1) time.
    //
    // (b) Design a data structure that uses O(n) space and answers queries in
    // O(log n) time. For partial credit, your data structure can use O(n log n)
    // space and have O(log n) query time.
}

// TODO
pub fn fn_12() {
    // Suppose you are given an input set S of n numbers, and a black box that
    // if given any sequence of real numbers and an integer k instantly and
    // correctly answers whether there is a subset of input sequence whose sum
    // is exactly k. Show how to use the black box O(n) times to find a subset
    // of S that adds up to k.
}

// TODO
pub fn fn_13() {
    // Let A[1..n] be an array of real numbers. Design an algorithm to perform
    // any sequence of the following operations:
    //
    // • Add(i,y) – Add the value y to the ith number.
    //
    // • Partial-sum(i) – Return the sum of the first i numbers, i.e. SUM(j=1 ->
    //  i, A[j]).
    //
    // There are no insertions or deletions; the only change is to the values of
    // the numbers. Each operation should take O(log n) steps. You may use one
    // additional array of size n as a work space.
}

// TODO
pub fn fn_14() {
    // Extend the data structure of the previous problem to support insertions
    // and deletions. Each element now has both a key and a value. An element is
    // accessed by its key. The addition operation is applied to the values, but
    // the elements are accessed by its key. The Partial sum operation is
    // different.
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
    // Design a data structure that allows one to search, insert, and delete an
    // integer X in O(1) time (i.e. , constant time, independent of the total
    // number of integers stored). Assume that 1 ≤ X ≤ n and that there are m +
    // n units of space available, where m is the maximum number of integers
    // that can be in the table at any one time. (Hint: use two arrays A[1..n]
    // and B[1..m].) You are not allowed to initialize either A or B, as that
    // would take O(m) or O(n) operations. This means the arrays are full of
    // random garbage to begin with, so you must be very careful.
}

// TODO
pub fn fn_16() {
    // Implement versions of several different dictionary data structures, such
    // as linked lists, binary trees, balanced binary search trees, and hash
    // tables. Conduct exper- iments to assess the relative performance of these
    // data structures in a simple ap- plication that reads a large text file
    // and reports exactly one instance of each word that appears within it.
    // This application can be efficiently implemented by main- taining a
    // dictionary of all distinct words that have appeared thus far in the text
    // and inserting/reporting each word that is not found. Write a brief report
    // with your conclusions.
}

// TODO
pub fn fn_17() {
    // A Caesar shift (see Section 18.6 (page 641)) is a very simple class of
    // ciphers for secret messages. Unfortunately, they can be broken using
    // statistical properties of English. Develop a program capable of
    // decrypting Caesar shifts of sufficiently long texts.
}

// TODO
pub fn fn_18() {
    // What method would you use to look up a word in a dictionary?
}

// TODO
pub fn fn_19() {
    // Imagine you have a closet full of shirts. What can you do to organize
    // your shirts for easy retrieval?
}

// TODO
pub fn fn_20() {
    // Write a function to find the middle node of a singly-linked list.
}

// TODO
pub fn fn_21() {
    // Write a function to compare whether two binary trees are identical.
    // Identical trees have the same key value at each position and the same
    // structure.
}

// TODO
pub fn fn_22() {
    // Write a program to convert a binary search tree into a linked list.
}

// TODO
pub fn fn_23() {
    // Implement an algorithm to reverse a linked list. Now do it without
    // recursion.
}

// TODO
pub fn fn_24() {
    // What is the best data structure for maintaining URLs that have been
    // visited by a Web crawler? Give an algorithm to test whether a given URL
    // has already been visited, optimizing both space and time.
}

// TODO
pub fn fn_25() {
    // You are given a search string and a magazine. You seek to generate all
    // the char- acters in search string by cutting them out from the magazine.
    // Give an algorithm to efficiently determine whether the magazine contains
    // all the letters in the search string.
}

// TODO
pub fn fn_26() {
    // Reverse the words in a sentence—i.e., “My name is Chris” becomes “Chris
    // is name My.” Optimize for time and space.
}

// TODO
pub fn fn_27() {
    // Determine whether a linked list contains a loop as quickly as possible
    // without using any extra storage. Also, identify the location of the loop.
}

// TODO
pub fn fn_28() {
    // You have an unordered array X of n integers. Find the array M containing
    // n elements where Mi is the product of all integers in X except for Xi .
    // You may not use division. You can use extra memory. (Hint: There are
    // solutions faster than O(n2 ).)
}

// TODO
pub fn fn_29() {
    // Give an algorithm for finding an ordered word pair (e.g., “New York”)
    // occurring with the greatest frequency in a given webpage. Which data
    // structures would you use? Optimize both time and space.
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
}
