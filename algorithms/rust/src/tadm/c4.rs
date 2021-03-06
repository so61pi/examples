pub fn fn_01(players: &mut [usize]) -> (&[usize], &[usize]) {
    // [3] The Grinch is given the job of partitioning 2n players into two teams of
    // n players each. Each player has a numerical rating that measures how good
    // he/she is at the game. He seeks to divide the players as unfairly as
    // possible, so as to create the biggest possible talent imbalance between team
    // A and team B. Show how the Grinch can do the job in O(n log n) time.

    // The word is unfairly so we can sort the players by their ratings and then
    // split them in the middle to 2 halves.

    assert_eq!(players.len() % 2, 0);
    players.sort_unstable();
    players.split_at(players.len() / 2)
}

pub fn fn_02() {
    // [3] For each of the following problems, give an algorithm that finds the
    // desired numbers within the given amount of time. To keep your answers
    // brief, feel free to use algorithms from the book as subroutines. For the
    // example, S = {6, 13, 19, 3, 8}, 19 − 3 maximizes the difference, while 8
    // − 6 minimizes the difference.
    //
    // (a) Let S be an unsorted array of n integers. Give an algorithm that
    // finds the pair x, y ∈ S that maximizes |x − y|. Your algorithm must run
    // in O(n) worst-case time.
    //
    // (b) Let S be a sorted array of n integers. Give an algorithm that finds
    // the pair x, y ∈ S that maximizes |x − y|. Your algorithm must run in O(1)
    // worst-case time.
    //
    // (c) Let S be an unsorted array of n integers. Give an algorithm that
    // finds the pair x, y ∈ S that minimizes |x − y|, for x != y. Your algorithm
    // must run in O(n log n) worst-case time.
    //
    // (d) Let S be a sorted array of n integers. Give an algorithm that finds
    // the pair x, y ∈ S that minimizes |x − y|, for x != y. Your algorithm must
    // run in O(n) worst-case time.

    // (a), (b) max|x - y| = max|max(array) - min(array)|
    //
    // (c), (d) sort then compute and compare all of (array[i+1] - array[i])
}

pub fn fn_03(numbers: &mut [i32]) -> Vec<(i32, i32)> {
    // [3] Take a sequence of 2n real numbers as input. Design an O(n log n)
    // algorithm that partitions the numbers into n pairs, with the property
    // that the partition minimizes the maximum sum of a pair. For example, say
    // we are given the numbers (1,3,5,9). The possible partitions are
    // ((1,3),(5,9)), ((1,5),(3,9)), and ((1,9),(3,5)). The pair sums for these
    // partitions are (4,14), (6,12), and (10,8). Thus the third partition has
    // 10 as its maximum sum, which is the minimum over the three partitions.

    // Sort then pair first element with last element iteratively until all
    // elements are processed.

    assert_eq!(numbers.len() % 2, 0);
    numbers.sort_unstable();
    let f = numbers.iter().cloned().take(numbers.len() / 2);
    let r = numbers.iter().cloned().rev().take(numbers.len() / 2);

    let mut v = vec![];
    v.extend(f.zip(r));
    v
}

pub fn fn_04() {
    // [3] Assume that we are given n pairs of items as input, where the first
    // item is a number and the second item is one of three colors (red, blue,
    // or yellow). Further assume that the items are sorted by number. Give an
    // O(n) algorithm to sort the items by color (all reds before all blues
    // before all yellows) such that the numbers for identical colors stay
    // sorted.
    //
    // For example: (1,blue), (3,red), (4,blue), (6,yellow), (9,red) should
    // become (3,red), (9,red), (1,blue), (4,blue), (6,yellow).

    // We need 2 loops and an additional array of n elements to contain result.
    // First loop iteration, we count how many items each color has. With that
    // information at hand, we can partition our result array to 3 parts and in
    // second iteration we put each element to its correct area.
    //
    // This is similar to bucket sort.
}

pub fn fn_05() {
    // [3] The mode of a set of numbers is the number that occurs most
    // frequently in the set. The set (4, 6, 2, 4, 3, 1) has a mode of 4. Give
    // an efficient and correct algorithm to compute the mode of a set of n
    // numbers.

    // Sort and count consecutively identical numbers.
}

pub fn fn_06() {
    // [3] Given two sets S1 and S2 (each of size n), and a number x, describe
    // an O(n log n) algorithm for finding whether there exists a pair of
    // elements, one from S1 and one from S2 , that add up to x. (For partial
    // credit, give a Θ(n^2) algorithm for this problem.)

    // First, sort both S1 and S2, which takes O(n log n). Then for each element
    // in S1, we find x - e in S2 using binary search. The loop is O(n) and the
    // search is O(log n) so we are still in acceptable time complexity
    // requirement.
}

pub fn fn_07() {
    // [3] Outline a reasonable method of solving each of the following
    // problems. Give the order of the worst-case complexity of your methods.
    //
    // (a) You are given a pile of thousands of telephone bills and thousands of
    // checks sent in to pay the bills. Find out who did not pay.
    //
    // (b) You are given a list containing the title, author, call number and
    // publisher of all the books in a school library and another list of 30
    // publishers. Find out how many of the books in the library were published
    // by each company.
    //
    // (c) You are given all the book checkout cards used in the campus library
    // during the past year, each of which contains the name of the person who
    // took out the book. Determine how many distinct people checked out at
    // least one book.

    // (a) Put the checks to a hashmap with the key is the phone number. Then
    // loop through the bills and check if it exists in the hashmap.
    //
    // Or we can sort the checks then binary search the bills.
    //
    // (b) Use hashmap with the key is publisher name and value is counter.
    //
    // (c) Put person's name in hash table and return the table's size at the
    // end.
}

pub fn fn_08(numbers: &mut [i32], x: i32) -> bool {
    // [4] Given a set of S containing n real numbers, and a real number x. We
    // seek an algorithm to determine whether two elements of S exist whose sum
    // is exactly x.
    //
    // (a) Assume that S is unsorted. Give an O(n log n) algorithm for the
    // problem.
    //
    // (b) Assume that S is sorted. Give an O(n) algorithm for the problem.

    // (a) Sort the array then go to (b).
    //
    // (b) Create an array S' whose elements are the result of x - S. Because S
    // is sorted, so is S' but in reverse order. We now basically search for a
    // pair of equal numbers in both S and S' but with different indice.

    fn skip_until<T, P>(i: &mut dyn std::iter::Iterator<Item = T>, predicate: P) -> Option<T>
    where
        P: Fn(&T) -> bool,
    {
        loop {
            let n = i.next();
            if n.is_none() || predicate(n.as_ref().unwrap()) {
                return n;
            }
        }
    }

    numbers.sort_unstable();
    let mut aux = vec![];
    aux.extend(numbers.iter().map(|n| x - n));

    let mut i1 = numbers.iter().enumerate(); // L -> R
    let mut i2 = aux.iter().rev().enumerate(); // R -> L

    let mut a = match i2.clone().next() {
        None => return false,
        Some(v) => v,
    };
    loop {
        let b = match skip_until(&mut i1, |(_, e)| a.1 <= *e) {
            None => break,
            Some(v) => v,
        };

        let c = match skip_until(&mut i2, |(_, e)| b.1 <= *e) {
            None => break,
            Some(v) => v,
        };

        // Same value but must be distinct numbers.
        if b.1 == c.1 && (b.0 + c.0 + 1 != numbers.len()) {
            return true;
        }

        a = c;
    }

    false
}

pub fn fn_09() {
    // [4] Give an efficient algorithm to compute the union of sets A and B,
    // where n = max(|A|, |B|). The output should be an array of distinct
    // elements that form the union of the sets, such that they appear more than
    // once in the union.
    //
    // (a) Assume that A and B are unsorted. Give an O(n log n) algorithm for
    // the problem.
    //
    // (b) Assume that A and B are sorted. Give an O(n) algorithm for the
    // problem.

    // (a) Sort then do (b).
    //
    // (b) Use skip_until in fn_08 to get all pairs of numbers with the same
    // value.
}

pub fn fn_10(numbers: &mut [i32], t: i32, k: usize) -> bool {
    // [5] Given a set S of n integers and an integer T , give an O(n^(k−1) log n)
    // algorithm to test whether k of the integers in S add up to T .

    // The strategy is to break this down to a smaller problem. The general idea is
    // we pick a number from the given array, and test the rest of the array (sub array)
    // with k - 1 and T - picked number, until we only have to pick one value (which
    // means k = 1).
    //
    // For example, with k = 3, T = 36, and array is [10, 11, 12, 13], we pick the
    // first element of result set initially assume the first element 10 is in the
    // result set, then we check sub problem which has k = 2, T = 36 - 10, and array
    // = [11, 12, 13]. If choosing 10 doesn't work out, we pick to the next value, 11,
    // as our first value of result set, and repeat the cycle.
    //
    // Note that we don't need to consider 10 in this second try, as it is proven that
    // 10 cannot be in the result from first run. Consequently, we have [10, 11, 12, 13]
    // on first run, but [11, 12, 13] on second run, and so on.
    //
    // Another way to solve this is to construct a matrix of k-1 dimensions, each element
    // is the sum of k-1 values selected from input array. Then we loop through the matrix,
    // and do binary searches with T-value.

    assert!(k > 0);
    assert!(numbers.len() >= k);
    numbers.sort_unstable();

    fn check(numbers: &[i32], first: usize, last: usize, t: i32, level: usize) -> bool {
        if level <= 1 {
            numbers[first..].binary_search(&t).is_ok()
        } else {
            for i in first..last {
                if check(numbers, i + 1, last, t - numbers[i], level - 1) {
                    return true;
                }
            }
            false
        }
    }

    check(&numbers, 0, numbers.len(), t, k)
}

pub fn fn_11() {
    // [6] Design an O(n) algorithm that, given a list of n elements, finds all
    // the elements that appear more than n/2 times in the list. Then, design an
    // O(n) algorithm that, given a list of n elements, finds all the elements
    // that appear more than n/4 times.

    // We can use O(n) selection algorithm (like nth_element in C++).
    //
    // Note that nth_element in C++ not only gets the nth smallest element, but
    // also partitions the array at nth so that "All of the elements before this
    // new nth element are less than or equal to the elements after the new nth
    // element.".
    //
    // https://en.wikipedia.org/wiki/Selection_algorithm
    // https://en.cppreference.com/w/cpp/algorithm/nth_element
}

pub fn fn_12() {
    // [3] Devise an algorithm for finding the k smallest elements of an
    // unsorted set of n integers in O(n + k log n).

    // nth_element
}

pub fn fn_13() {
    // [5] You wish to store a set of n numbers in either a max-heap or a sorted
    // array. For each application below, state which data structure is better,
    // or if it does not matter. Explain your answers.
    //
    // (a) Want to find the maximum element quickly.
    //
    // (b) Want to be able to delete an element quickly.
    //
    // (c) Want to be able to form the structure quickly.
    //
    // (d) Want to find the minimum element quickly.

    // (a) Doesn't matter.
    //
    // (b) Sorted array.
    //
    // (c) max-heap
    //
    // (d) Sorted array
}

pub fn fn_14(lists: &[&[i32]]) -> Vec<i32> {
    // [5] Give an O(n log k)-time algorithm that merges k sorted lists with a
    // total of n elements into one sorted list. (Hint: use a heap to speed up
    // the elementary O(kn)- time algorithm).

    use std::collections::BinaryHeap;
    let mut heap = BinaryHeap::new();

    #[derive(Debug)]
    struct Elem {
        value: i32,
        index: usize,
        ilist: usize,
    };

    impl PartialEq for Elem {
        fn eq(&self, other: &Elem) -> bool {
            self.value.eq(&other.value)
        }
    }

    impl Eq for Elem {}

    impl PartialOrd for Elem {
        fn partial_cmp(&self, other: &Elem) -> Option<std::cmp::Ordering> {
            self.value.partial_cmp(&other.value)
        }
    }

    impl Ord for Elem {
        fn cmp(&self, other: &Elem) -> std::cmp::Ordering {
            self.value.cmp(&other.value)
        }
    }

    for (ilist, list) in lists.iter().enumerate() {
        if let Some(value) = list.last() {
            heap.push(Elem {
                value: *value,
                index: list.len() - 1,
                ilist: ilist,
            });
        }
    }

    let mut v = vec![];
    while let Some(ref elem) = heap.pop() {
        v.push(elem.value);

        if elem.index > 0 {
            heap.push(Elem {
                value: lists[elem.ilist][elem.index - 1],
                index: elem.index - 1,
                ilist: elem.ilist,
            });
        }
    }

    v.reverse();
    v
}

pub fn fn_15() {
    // [5] (a) Give an efficient algorithm to find the second-largest key among
    // n keys. You can do better than 2n − 3 comparisons. (b) Then, give an
    // efficient algorithm to find the third-largest key among n keys. How many
    // key comparisons does your algorithm do in the worst case? Must your
    // algorithm determine which key is largest and second-largest in the
    // process?

    // (a) Pick first two elements as the largest and second-largest (this takes
    // one comparision), then sequentially pick new element and check if it is
    // the new largest or second-largest one. 2(n-2) + 1 = 2n - 3 checks in
    // total.
    //
    // (b) Heapify the array then select the third-largest element.
    //
    // We could also use tournament algorithm.
}

pub fn fn_16(data: &mut [i64]) -> Option<i64> {
    // [3] Use the partitioning idea of quicksort to give an algorithm that
    // finds the median element of an array of n integers in expected O(n) time.
    // (Hint: must you look at both sides of the partition?)

    if data.is_empty() {
        return None;
    }

    let (mut first, mut last) = (0, data.len());
    let mid = (last - first) / 2;
    loop {
        let localmid = (last - first) / 2 + first;
        let localval = data[localmid];
        let point1 = itertools::partition(&mut data[first..last], |e| *e < localval) + first;
        let point2 = itertools::partition(&mut data[point1..last], |e| !(localval < *e)) + point1;

        if first <= mid && mid < point1 {
            last = point1;
        }
        if point1 <= mid && mid < point2 {
            return Some(localval);
        }
        if point2 <= mid && mid < last {
            first = point2;
        }
    }
}

// TODO: Skip
pub fn fn_17() {
    // [3] The median of a set of n values is the ┌ n/2 ┐ th smallest value.
    //
    // (a) Suppose quicksort always pivoted on the median of the current
    // sub-array. How many comparisons would Quicksort make then in the worst
    // case?
    //
    // (b) Suppose quicksort were always to pivot on the ┌ n/3 ┐ th smallest
    // value of the current sub-array. How many comparisons would be made then
    // in the worst case?
}

pub fn fn_18<'a, T>(
    data: &'a mut [T],
    red: &dyn Fn(&T) -> bool,
    blue: &dyn Fn(&T) -> bool,
) -> &'a [T] {
    // [5] Suppose an array A consists of n elements, each of which is
    // red, white, or blue. We seek to sort the elements so that all the reds
    // come before all the whites, which come before all the blues The only
    // operation permitted on the keys are
    //
    // • Examine(A,i) – report the color of the ith element of A.
    //
    // • Swap(A,i,j) – swap the ith element of A with the jth element.
    //
    // Find a correct and efficient algorithm for red-white-blue sorting. There
    // is a linear- time solution.

    // - Move all red elements to the beginning by partitioning O(n).
    // - Move all none-blue (white) elements to the beginning by partitioning O(n).
    // - Blue ones are at the end now.

    let point = itertools::partition(&mut data[..], red);
    itertools::partition(&mut data[point..], |e| !blue(e));
    data
}

// TODO: Skip
pub fn fn_19() {
    // [5] An inversion of a permutation is a pair of elements that are out of
    // order.
    //
    // (a) Show that a permutation of n items has at most n(n − 1)/2 inversions.
    // Which permutation(s) have exactly n(n − 1)/2 inversions?
    //
    // (b) Let P be a permutation and Pr be the reversal of this permutation.
    // Show that P and Pr have a total of exactly n(n − 1)/2 inversions.
    //
    // (c) Use the previous result to argue that the expected number of
    // inversions in a random permutation is n(n − 1)/4.
}

pub fn fn_20(data: &mut [i64]) -> &[i64] {
    // [3] Give an efficient algorithm to rearrange an array of n keys so that
    // all the negative keys precede all the nonnegative keys. Your algorithm
    // must be in-place, meaning you cannot allocate another array to
    // temporarily hold the items. How fast is your algorithm?

    // One partitioning round is enough. O(n)
    itertools::partition(&mut data[..], |e| *e < 0);
    data
}

pub fn fn_21() {
    // [5] Stable sorting algorithms leave equal-key items in the same relative
    // order as in the original permutation. Explain what must be done to ensure
    // that mergesort is a stable sorting algorithm.

    // When merging equal elements, pick the ones on the left first.
}

pub fn fn_22() {
    // [3] Show that n positive integers in the range 1 to k can be sorted in
    // O(n log k) time. The interesting case is when k << n.

    // Build a binary search tree with each node contains a key from 1 to k and
    // an array. Same elements go into same node. Looping through the array
    // takes O(n), seaching for the right node takes O(log k). Constructing
    // sorted array from built tree takes O(n).
}

pub fn fn_23() {
    // [5] We seek to sort a sequence S of n integers with many duplications,
    // such that the number of distinct integers in S is O(log n). Give an O(n
    // log log n) worst-case time algorithm to sort such sequences.

    // Same as above, k = log n.
}

pub fn fn_24() {
    // [5] Let A[1..n] be an array such that the first n − √n elements are
    // already sorted (though we know nothing about the remaining elements).
    // Give an algorithm that sorts A in substantially better than n log n
    // steps.

    // First sorts √n elements at the end, this takes √n log √n. Then merges 2
    // sorted pieces together, takes n. So in total it takes n + √n log √n. When
    // n > 2, n + √n log √n is less then n log n.
}

pub fn fn_25() {
    // [5] Assume that the array A[1..n] only has numbers from {1, . . . , n^2}
    // but that at most log log n of these numbers ever appear. Devise an
    // algorithm that sorts A in substantially less than O(n log n).

    // Same as fn_22, k = log log n, hence O(n log log log n).
}

pub fn fn_26() {
    // [5] Consider the problem of sorting a sequence of n 0’s and 1’s using
    // comparisons. For each comparison of two values x and y, the algorithm
    // learns which of x < y, x = y, or x > y holds.
    //
    // (a) Give an algorithm to sort in n − 1 comparisons in the worst case.
    // Show that your algorithm is optimal.
    //
    // (b) Give an algorithm to sort in 2n/3 comparisons in the average case
    // (assuming each of the n inputs is 0 or 1 with equal probability). Show
    // that your algorithm is optimal.

    // (a) Compare elements from 2nd position (y) to the first one (x)
    //
    // - If y > x => put y to bucket 3
    // - If y = x => put y to bucket 2
    // - If y < x => put y to bucket 1
    //
    // Total number of comparisons is n - 1
    //
    // (b) Compare elements in pair ([0] is compared with [1], [2] is compared
    // with [3], [4] is compared with [5]...) with the same rules as (a). If y =
    // x, we need another compare to detect whether they are 0 or 1.
    //
    // So we have n/2 comparisons at first, one third of them need an additional
    // comparisions. In total, (n/2)/3 + (n/2)/3 + (n/2)/3*2 = (n/2 + n/2 + n)/3
    // = (2n)/3 comparisions are nedded.
}

// TODO
pub fn fn_27() {
    // [6] Let P be a simple, but not necessarily convex, polygon and q an
    // arbitrary point not necessarily in P . Design an efficient algorithm to
    // find a line segment originating from q that intersects the maximum number
    // of edges of P . In other words, if standing at point q, in what direction
    // should you aim a gun so the bullet will go through the largest number of
    // walls. A bullet through a vertex of P gets credit for only one wall. An
    // O(n log n) algorithm is possible.
}

// TODO: Skip
pub fn fn_28() {
    // [5] In one of my research papers  [Ski88], I discovered a
    // comparison-based sorting algorithm that runs in O(n log(√n)). Given the
    // existence of an Ω(n log n) lower bound for sorting, how can this be
    // possible?
}

// TODO: Skip
pub fn fn_29() {
    // [5] Mr. B. C. Dull claims to have developed a new data structure for
    // priority queues that supports the operations Insert, Maximum, and
    // Extract-Max—all in O(1) worst-case time. Prove that he is mistaken.
    // (Hint: the argument does not involve a lot of gory details—just think
    // about what this would imply about the Ω(n log n) lower bound for
    // sorting.)
}

// TODO: Skip
pub fn fn_30() {
    // [3] A company database consists of 10,000 sorted names, 40% of whom are
    // known as good customers and who together account for 60% of the accesses
    // to the database. There are two data structure options to consider for
    // representing the database:
    //
    // • Put all the names in a single array and use binary search.
    //
    // • Put the good customers in one array and the rest of them in a second
    // array. Only if we do not find the query name on a binary search of the
    // first array do we do a binary search of the second array.
    //
    // Demonstrate which option gives better expected performance. Does this
    // change if linear search on an unsorted array is used instead of binary
    // search for both options?
}

pub fn fn_31() {
    // [3] Suppose you are given an array A of n sorted numbers that has been
    // circularly shifted k positions to the right. For example, {35, 42, 5, 15,
    // 27, 29} is a sorted array that has been circularly shifted k = 2
    // positions, while {27, 29, 35, 42, 5, 15} has been shifted k = 4
    // positions.
    //
    // • Suppose you know what k is. Give an O(1) algorithm to find the largest
    // number in A.
    //
    // • Suppose you do not know what k is. Give an O(lg n) algorithm to find
    // the largest number in A. For partial credit, you may give an O(n)
    // algorithm.

    // - A[k-1]
    //
    // - Binary search solution is online possible when the numbers are unique.
    // Otherwise we have to scan array linearly (e.g. [2, 2, 2, 2, 2, 3, 1, 2])
}

pub fn fn_32() {
    // [3] Consider the numerical 20 Questions game. In this game, Player 1
    // thinks of a number in the range 1 to n. Player 2 has to figure out this
    // number by asking the fewest number of true/false questions. Assume that
    // nobody cheats.
    //
    // (a) What is an optimal strategy if n in known?
    //
    // (b) What is a good strategy is n is not known?

    // (a) Binary search.
    //
    // (b) Ask each digit of picked number in binary form
    // (Is the first digit 1? Is the second digit 1?...).
}

pub fn fn_33() {
    // [5] Suppose that you are given a sorted sequence of distinct integers {a1
    // , a2 , . . . , an}. Give an O(lg n) algorithm to determine whether there
    // exists an i index such as ai = i. For example, in {−10, −3, 3, 5, 7}, a3
    // = 3. In {2, 3, 4, 5, 6, 7}, there is no such i.

    // Binary search
    //
    // if a[mid] == mid
    //     done
    // if a[mid] > mid
    //     search(left)
    // if a[mid] < mid
    //     search(right)
}

pub fn fn_34() {
    // [5] Suppose that you are given a sorted sequence of distinct integers {a1
    // , a2 , . . . , an}, drawn from 1 to m where n < m. Give an O(lg n)
    // algorithm to find an integer ≤ m that is not present in a. For full
    // credit, find the smallest such integer.

    // Binary search
    //
    // if a[mid] >= m/2
    //     search(left)
    // if a[mid] < m/2
    //     search(right)
}

pub fn fn_35() {
    // [5] Let M be an n × m integer matrix in which the entries of each row are
    // sorted in increasing order (from left to right) and the entries in each
    // column are in increasing order (from top to bottom). Give an efficient
    // algorithm to find the position of an integer x in M , or to determine
    // that x is not there. How many comparisons of x with matrix entries does
    // your algorithm use in worst case?

    /*
                m=10
        0 1 2 3 4 5 6 7 8 9
        1 2 3 4 5 6 7 8 9 A
        2 3 4 5 6 7 8 9 A B
    n=7 3 4 5 6 7 8 9 A B C
        4 5 6 7 8 9 A B C D
        5 6 7 8 9 A B C D E
        6 7 8 9 A B C D E F

    First way
        search
            a . . b
            . . . .
            . . . c

            a = M[0;0]
            b = M[0;9]
            c = M[6;9]

            if x = a or b or c -> true

            if a < x < b
                C = binary search for column where x is in
                search x in C
            if b < x < c
                R = binary search for row where x is in
                search x in R

    Second way
        v > A  <=>  !(v <= A)
                        *
            . . . . . . . 7 8 9
            . . . . . . . 8 9 A
            . . . . . . . 9 A B
            . . . . . . . A B C
          * . . . . . . . B C D
            5 6 7 8 9 A B C D E
            6 7 8 9 A B C D E F

        v < A  <=>  !(v >= A)
                        *
            0 1 2 3 4 5 6 7 8 9
            1 2 3 4 5 6 7 8 9 A
            2 3 4 5 6 7 8 9 A B
            3 4 5 6 7 8 9 A B C
          * 4 5 6 7 8 9 . . . .
            5 6 7 8 9 A . . . .
            6 7 8 9 A B . . . .

        * = mid

        search [rmin rmax), [cmin cmax)
            if rmin == rmax || cmin == cmax -> false

            rmid = (rmax - rmin) / 2 + rmin
            cmid = (cmax - cmin) / 2 + cmin
            e = M[rmid][cmid]

            if x = e -> true
            if x > e
                search rmin rmid+1, cmid+1 cmax
                search rmid+1 rmax, cmin cmid+1
                search rmid+1 rmax, cmid+1 cmax
            if x < e
                search rmin rmid, cmin cmid
                search rmin rmid, cmid cmax
                search rmid rmax, cmin cmid
    */

}

pub fn fn_36() {
    // [5] Consider an n × n array A containing integer elements (positive,
    // negative, and zero). Assume that the elements in each row of A are in
    // strictly increasing order, and the elements of each column of A are in
    // strictly decreasing order. (Hence there cannot be two zeroes in the same
    // row or the same column.) Describe an efficient algorithm that counts the
    // number of occurrences of the element 0 in A. Analyze its running time.

    /*
        | >e | >e | >e
    ----+----+----+----
     <e |  e | >e | >e
    ----+----+----+----
     <e | <e |    |
    ----+----+----+----
     <e | <e |    |

      e | >e | >e
    ----+----+----
     <e |    |
    ----+----+----
     <e |    |

    Values in empty cells can be equal to e.

    e = A[r][c]
    if e = 0
        counter++
        check r+1,c+1
    if e < 0
        check r, c+1
    if e > 0
        check r+1, c
    */
}

// TODO: Skip
pub fn fn_37() {
    // [6] Implement versions of several different sorting algorithms, such as
    // selection sort, insertion sort, heapsort, mergesort, and quicksort.
    // Conduct experiments to assess the relative performance of these
    // algorithms in a simple application that reads a large text file and
    // reports exactly one instance of each word that appears within it. This
    // application can be efficiently implemented by sorting all the words that
    // occur in the text and then passing through the sorted sequence to
    // identify one instance of each distinct word. Write a brief report with
    // your conclusions.
}

pub fn fn_38() {
    // [5] Implement an external sort, which uses intermediate files to sort
    // files bigger than main memory. Mergesort is a good algorithm to base such
    // an implementation on. Test your program both on files with small records
    // and on files with large records.

    // Similar to fn_43
}

// TODO: Skip
pub fn fn_39() {
    // [8] Design and implement a parallel sorting algorithm that distributes
    // data across several processors. An appropriate variation of mergesort is
    // a likely candidate. Mea- sure the speedup of this algorithm as the number
    // of processors increases. Later, compare the execution time to that of a
    // purely sequential mergesort implementa- tion. What are your experiences?
}

pub fn fn_40() {
    // [3] If you are given a million integers to sort, what algorithm would you
    // use to sort them? How much time and memory would that consume?

    // Quick sort.
}

// TODO: Skip
pub fn fn_41() {
    // [3] Describe advantages and disadvantages of the most popular sorting
    // algorithms.
}

pub fn fn_42() {
    // [3] Implement an algorithm that takes an input array and returns only the
    // unique elements in it.

    // Sort and remove duplicates.
}

pub fn fn_43() {
    // [5] You have a computer with only 2Mb of main memory. How do you use it
    // to sort a large file of 500 Mb that is on disk?

    // Merge sort with buffer of 2Mb. Basically we have to merge 2 sorted arrays,
    // each is larger than the provided buffer.
    //
    // Things get complicated when we have to reduce the number of trips between
    // RAM and disk.
}

pub fn fn_44() {
    // [5] Design a stack that supports push, pop, and retrieving the minimum
    // element in constant time. Can you do this?

    // We need additional stack to keep the minimum values.
    //
    // When new element is pushed on main stack, also push it to min stack if
    // it's smaller than current value of min stack.
    //
    // When element is poped from main stack, also pop min stack if the being
    // poped element of main stack equals to the one on min stack.
}

pub fn fn_45() {
    // [5] Given a search string of three words, find the smallest snippet of
    // the document that contains all three of the search words - i.e., the
    // snippet with smallest number of words in it. You are given the index
    // positions where these words occur in the document, such as word1: (1, 4,
    // 5), word2: (3, 9, 10), and word3: (2, 6, 15). Each of the lists are in
    // sorted order, as above.

    // 123456789ABCDEF
    // x  xx
    //   y     yy
    //  z   z        z
    //
    // xzyxxz  yy    z -> mergedArray
    //
    // (lastX, lastY, lastZ) = (1, 3, 2)
    // for i in mergedArray[4:] {
    //   switch i {
    //     x: update lastX
    //     y: update lastY
    //     z: update lastZ
    //   }
    //   best = min(best, (lastX, lastY, lastZ))
    // }
}

// TODO: Skip
pub fn fn_46() {
    // [6] You are given 12 coins. One of them is heavier or lighter than the
    // rest. Identify this coin in just three weighings.
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_fn_08() {
        assert!(!fn_08(&mut [1, 4, 3, 2], 1));
        assert!(!fn_08(&mut [1, 4, 3, 2], 2));
        assert!(fn_08(&mut [1, 4, 3, 2], 3));
        assert!(fn_08(&mut [1, 2, 3, 4], 4));
        assert!(fn_08(&mut [1, 2, 3, 4], 5));
        assert!(fn_08(&mut [1, 2, 3, 4], 6));
        assert!(fn_08(&mut [1, 2, 3, 4], 7));
        assert!(!fn_08(&mut [1, 2, 3, 4], 8));
        assert!(!fn_08(&mut [1, 2, 3, 4], 9));
    }

    #[test]
    fn test_fn_10() {
        assert!(!fn_10(&mut [1, 2, 3, 4], 0, 2));
        assert!(!fn_10(&mut [1, 2, 3, 4], 1, 2));
        assert!(!fn_10(&mut [1, 2, 3, 4], 2, 2));
        assert!(fn_10(&mut [1, 2, 3, 4], 3, 2));
        assert!(fn_10(&mut [1, 2, 3, 4], 4, 2));
        assert!(fn_10(&mut [1, 2, 3, 4], 5, 2));
        assert!(fn_10(&mut [1, 2, 3, 4], 6, 2));
        assert!(fn_10(&mut [1, 2, 3, 4], 7, 2));
        assert!(!fn_10(&mut [1, 2, 3, 4], 8, 2));
    }

    #[test]
    fn test_fn_14() {
        assert_eq!(fn_14(&[&[0, 2, 4], &[1, 3, 5]]), &[0, 1, 2, 3, 4, 5]);
        assert_eq!(fn_14(&[&[0, 2, 4], &[1, 3, 5], &[]]), &[0, 1, 2, 3, 4, 5]);
        assert_eq!(fn_14(&[&[0, 2, 4], &[1, 5], &[3]]), &[0, 1, 2, 3, 4, 5]);
    }

    #[test]
    fn test_fn_16() {
        assert_eq!(fn_16(&mut []), None);
        assert_eq!(fn_16(&mut [1]), Some(1));
        assert_eq!(fn_16(&mut [1, 2]), Some(2));
        assert_eq!(fn_16(&mut [2, 1]), Some(2));
        assert_eq!(fn_16(&mut [1, 2, 3]), Some(2));
        assert_eq!(fn_16(&mut [3, 2, 1]), Some(2));
        assert_eq!(fn_16(&mut [1, 2, 3, 4]), Some(3));
        assert_eq!(fn_16(&mut [4, 3, 2, 1]), Some(3));
        assert_eq!(fn_16(&mut [1, 2, 3, 4, 5]), Some(3));
        assert_eq!(fn_16(&mut [5, 4, 3, 2, 1]), Some(3));

        assert_eq!(fn_16(&mut [1, 1, 1, 1]), Some(1));
        assert_eq!(fn_16(&mut [1, 2, 2, 2]), Some(2));
        assert_eq!(fn_16(&mut [2, 2, 2, 1]), Some(2));
        assert_eq!(fn_16(&mut [1, 2, 2, 1]), Some(2));
        assert_eq!(fn_16(&mut [1, 2, 1, 2]), Some(2));
    }

    #[test]
    fn test_fn_18() {
        fn red(val: &i64) -> bool {
            *val < 0
        }

        fn blue(val: &i64) -> bool {
            0 < *val
        }

        assert_eq!(fn_18(&mut [], &red, &blue), []);
        assert_eq!(fn_18(&mut [-1], &red, &blue), [-1]);
        assert_eq!(fn_18(&mut [-1, 0], &red, &blue), [-1, 0]);
        assert_eq!(fn_18(&mut [-1, 0, 1], &red, &blue), [-1, 0, 1]);
        assert_eq!(fn_18(&mut [1, 0, -1], &red, &blue), [-1, 0, 1]);
        assert_eq!(fn_18(&mut [1, -1, 0], &red, &blue), [-1, 0, 1]);
    }
}
