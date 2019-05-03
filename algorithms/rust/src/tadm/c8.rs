// Edit Distance
// TODO
pub fn fn_01() {
    // [3] Typists often make transposition errors exchanging neighboring
    // characters, such as typing “setve” when you mean “steve.” This requires
    // two substitutions to fix under the conventional definition of edit
    // distance. Incorporate a swap operation into our edit distance function,
    // so that such neighboring transposition errors can be fixed at the cost of
    // one operation.
}

// TODO
pub fn fn_02() {
    // [4] Suppose you are given three strings of characters: X, Y , and Z,
    // where |X| = n, |Y| = m, and |Z| = n + m. Z is said to be a shuffle of X
    // and Y iff Z can be formed by interleaving the characters from X and Y in
    // a way that maintains the left-to-right ordering of the characters from
    // each string.
    //
    // (a) Show that cchocohilaptes is a shuffle of chocolate and chips, but
    // chocochilatspe is not.
    //
    // (b) Give an efficient dynamic-programming algorithm that determines
    // whether Z is a shuffle of X and Y . Hint: the values of the dynamic
    // programming matrix you construct should be Boolean, not numeric.
}

// TODO
pub fn fn_03() {
    // [4] The longest common substring (not subsequence) of two strings X and Y is
    // the longest string that appears as a run of consecutive letters in both strings. For
    // example, the longest common substring of photograph and tomography is ograph.
    //
    // (a) Let n = |X| and m = |Y|. Give a Θ(nm) dynamic programming algorithm
    // for longest common substring based on the longest common subsequence/edit
    // distance algorithm.
    //
    // (b) Give a simpler Θ(nm) algorithm that does not rely on dynamic programming.
}

// TODO
pub fn fn_04() {
    // [6] The longest common subsequence (LCS) of two sequences T and P is the
    // longest sequence L such that L is a subsequence of both T and P . The
    // shortest common supersequence (SCS) of T and P is the smallest sequence L
    // such that both T and P are a subsequence of L.
    //
    // (a) Give efficient algorithms to find the LCS and SCS of two given
    // sequences.
    //
    // (b) Let d(T, P ) be the minimum edit distance between T and P when no
    // sub- stitutions are allowed (i.e., the only changes are character
    // insertion and dele- tion). Prove that d(T, P ) = |SCS(T, P )| − |LCS(T, P
    // )| where |SCS(T, P )| (|LCS(T, P )|) is the size of the shortest SCS
    // (longest LCS) of T and P .
}

// Greedy Algorithms
// TODO
pub fn fn_05() {
    // [4] Let P1 , P2, . . . , Pn be n programs to be stored on a disk with
    // capacity D megabytes. Program Pi requires si megabytes of storage. We
    // cannot store them all because D < SUM(i = 1 -> n, si)
    //
    // (a) Does a greedy algorithm that selects programs in order of
    // nondecreasing si maximize the number of programs held on the disk? Prove
    // or give a counter- example.
    //
    // (b) Does a greedy algorithm that selects programs in order of
    // nonincreasing order si use as much of the capacity of the disk as
    // possible? Prove or give a counter- example.
}

// TODO
pub fn fn_06() {
    // [5] Coins in the United States are minted with denominations of 1, 5, 10,
    // 25, and 50 cents. Now consider a country whose coins are minted with
    // denominations of {d1 , . . . , d k } units. We seek an algorithm to make
    // change of n units using the minimum number of coins for this country.
    //
    // (a) The greedy algorithm repeatedly selects the biggest coin no bigger
    // than the amount to be changed and repeats until it is zero. Show that the
    // greedy algorithm does not always use the minimum number of coins in a
    // country whose denominations are {1, 6, 10}.
    //
    // (b) Give an efficient algorithm that correctly determines the minimum
    // number of coins needed to make change of n units using denominations {d1
    // , . . . , dk }. Analyze its running time.
}

// TODO
pub fn fn_07() {
    // [5] In the United States, coins are minted with denominations of 1, 5,
    // 10, 25, and 50 cents. Now consider a country whose coins are minted with
    // denominations of {d1 , . . . , dk } units. We want to count how many
    // distinct ways C(n) there are to make change of n units. For example, in a
    // country whose denominations are {1, 6, 10}, C(5) = 1, C(6) to C(9) = 2,
    // C(10) = 3, and C(12) = 4.
    //
    // (a) How many ways are there to make change of 20 units from {1, 6, 10}?
    //
    // (b) Give an efficient algorithm to compute C(n), and analyze its
    // complexity. (Hint: think in terms of computing C(n, d), the number of
    // ways to make change of n units with highest denomination d. Be careful to
    // avoid overcount- ing.)
}

// TODO
pub fn fn_08() {
    // [6] In the single-processor scheduling problem, we are given a set of n
    // jobs J. Each job i has a processing time ti , and a deadline di . A
    // feasible schedule is a permutation of the jobs such that when the jobs
    // are performed in that order, every job is finished before its deadline.
    // The greedy algorithm for single-processor scheduling selects the job with
    // the earliest deadline first. Show that if a feasible schedule exists,
    // then the schedule produced by this greedy algorithm is feasible.
}

// Number Problems
// TODO
pub fn fn_09() {
    // [6] The knapsack problem is as follows: given a set of integers S = {s1 ,
    // s2 , . . . , sn }, and a given target number T , find a subset of S that
    // adds up exactly to T . For example, within S = {1, 2, 5, 9, 10} there is
    // a subset that adds up to T = 22 but not T = 23. Give a correct
    // programming algorithm for knapsack that runs in O(nT ) time.
}

// TODO
pub fn fn_11() {
    // [5] Assume that there are n numbers (some possibly negative) on a circle,
    // and we wish to find the maximum contiguous sum along an arc of the
    // circle. Give an efficient algorithm for solving this problem.
}

// TODO
pub fn fn_12() {
    // [5] A certain string processing language allows the programmer to break a
    // string into two pieces. It costs n units of time to break a string of n
    // characters into two pieces, since this involves copying the old string. A
    // programmer wants to break a string into many pieces, and the order in
    // which the breaks are made can affect the total amount of time used. For
    // example, suppose we wish to break a 20-character string after characters
    // 3, 8, and 10. If the breaks are made in left-right order, then the first
    // break costs 20 units of time, the second break costs 17 units of time,
    // and the third break costs 12 units of time, for a total of 49 steps. If
    // the breaks are made in right-left order, the first break costs 20 units
    // of time, the second break costs 10 units of time, and the third break
    // costs 8 units of time, for a total of only 38 steps. Give a dynamic
    // programming algorithm that takes a list of character positions after
    // which to break and determines the cheapest break cost in O(n3 ) time.
}

// TODO
pub fn fn_13() {
    // [5] Consider the following data compression technique. We have a table of
    // m text strings, each at most k in length. We want to encode a data string
    // D of length n using as few text strings as possible. For example, if our
    // table contains (a,ba,abab,b) and the data string is bababbaababa, the
    // best way to encode it is (b,abab,ba,abab,a)— a total of five code words.
    // Give an O(nmk) algorithm to find the length of the best encoding. You may
    // assume that every string has at least one encoding in terms of the table.
}

// TODO
pub fn fn_14() {
    // [5] The traditional world chess championship is a match of 24 games. The
    // current champion retains the title in case the match is a tie. Each game
    // ends in a win, loss, or draw (tie) where wins count as 1, losses as 0,
    // and draws as 1/2. The players take turns playing white and black. White
    // has an advantage, because he moves first. The champion plays white in the
    // first game. He has probabilities ww , wd , and wl of winning, drawing,
    // and losing playing white, and has probabilities bw , bd , and bl of
    // winning, drawing, and losing playing black.
    //
    // (a) Write a recurrence for the probability that the champion retains the
    // title. Assume that there are g games left to play in the match and that
    // the champion needs to win i games (which may end in a 1/2).
    //
    // (b) Based on your recurrence, give a dynamic programming to calculate the
    // cham- pion’s probability of retaining the title.
    //
    // (c) Analyze its running time for an n game match.
}

// TODO
pub fn fn_15() {
    // [8] Eggs break when dropped from great enough height. Specifically, there
    // must be a floor f in any sufficiently tall building such that an egg
    // dropped from the f th floor breaks, but one dropped from the (f − 1)st
    // floor will not. If the egg always breaks, then f = 1. If the egg never
    // breaks, then f = n + 1. You seek to find the critical floor f using an
    // n-story building. The only operation you can perform is to drop an egg
    // off some floor and see what happens. You start out with k eggs, and seek
    // to drop eggs as few times as possible. Broken eggs cannot be reused. Let
    // E(k, n) be the minimum number of egg droppings that will always suffice.
    //
    // (a) Show that E(1, n) = n.
    //
    // (b) Show that E(k, n) = Θ(n^1/k).
    //
    // (c) Find a recurrence for E(k, n). What is the running time of the
    // dynamic pro- gram to find E(k, n)?
}

// Graph Problems
// TODO
pub fn fn_16() {
    // [4] Consider a city whose streets are defined by an X × Y grid. We are
    // interested in walking from the upper left-hand corner of the grid to the
    // lower right-hand corner. Unfortunately, the city has bad neighborhoods,
    // whose intersections we do not want to walk in. We are given an X × Y
    // matrix BAD, where BAD[i,j] = “yes” if and only if the intersection
    // between streets i and j is in a neighborhood to avoid.
    //
    // (a) Give an example of the contents of BAD such that there is no path
    // across the grid avoiding bad neighborhoods.
    //
    // (b) Give an O(XY ) algorithm to find a path across the grid that avoids
    // bad neigh- borhoods.
    //
    // (c) Give an O(XY ) algorithm to find the shortest path across the grid
    // that avoids bad neighborhoods. You may assume that all blocks are of
    // equal length. For partial credit, give an O(X^2*Y^2) algorithm.
}

// TODO
pub fn fn_17() {
    // [5] Consider the same situation as the previous problem. We have a city
    // whose streets are defined by an X × Y grid. We are interested in walking
    // from the upper left-hand corner of the grid to the lower right-hand
    // corner. We are given an X × Y matrix BAD, where BAD[i,j] = “yes” if and
    // only if the intersection between streets i and j is somewhere we want to
    // avoid. If there were no bad neighborhoods to contend with, the shortest
    // path across the grid would have length (X − 1) + (Y − 1) blocks, and
    // indeed there would be many such paths across the grid. Each path would
    // consist of only rightward and downward moves. Give an algorithm that
    // takes the array BAD and returns the number of safe paths of length X + Y
    // − 2. For full credit, your algorithm must run in O(XY ).
}

// Design Problems
// TODO
pub fn fn_18() {
    // [4] Consider the problem of storing n books on shelves in a library. The
    // order of the books is fixed by the cataloging system and so cannot be
    // rearranged. Therefore, we can speak of a book bi , where 1 ≤ i ≤ n, that
    // has a thickness ti and height hi . The length of each bookshelf at this
    // library is L. Suppose all the books have the same height h (i.e. , h = hi
    // = hj for all i, j) and the shelves are all separated by a distance of
    // greater than h, so any book fits on any shelf. The greedy algorithm would
    // fill the first shelf with as many books as we can until we get the
    // smallest i such that bi does not fit, and then repeat with subsequent
    // shelves. Show that the greedy algorithm always finds the optimal shelf
    // placement, and analyze its time complexity.
}

// TODO
pub fn fn_19() {
    // [6] This is a generalization of the previous problem. Now consider the
    // case where the height of the books is not constant, but we have the
    // freedom to adjust the height of each shelf to that of the tallest book on
    // the shelf. Thus the cost of a particular layout is the sum of the heights
    // of the largest book on each shelf.
    //
    // • Give an example to show that the greedy algorithm of stuffing each
    // shelf as full as possible does not always give the minimum overall
    // height.
    //
    // • Give an algorithm for this problem, and analyze its time complexity.
    // Hint: use dynamic programming.
}

// TODO
pub fn fn_20() {
    // [5] We wish to compute the laziest way to dial given n-digit number on a
    // standard push-button telephone using two fingers. We assume that the two
    // fingers start out on the * and # keys, and that the effort required to
    // move a finger from one button to another is proportional to the Euclidean
    // distance between them. Design an algorithm that computes the method of
    // dialing that involves moving your fingers the smallest amount of total
    // distance, where k is the number of distinct keys on the keypad (k = 16
    // for standard telephones). Try to use O(nk 3 ) time.
}

// TODO
pub fn fn_21() {
    // [6] Given an array of n real numbers, consider the problem of finding the
    // maximum sum in any contiguous subvector of the input. For example, in the
    // array {31, −41, 59, 26, −53, 58, 97, −93, −23, 84} the maximum is
    // achieved by summing the third through seventh elements, where 59 + 26 +
    // (−53) + 58 + 97 = 187. When all numbers are positive, the entire array is
    // the answer, while when all numbers are negative, the empty array
    // maximizes the total at 0.
    //
    // • Give a simple, clear, and correct Θ(n2 )-time algorithm to find the
    // maximum contiguous subvector.
    //
    // • Now give a Θ(n)-time dynamic programming algorithm for this problem. To
    // get partial credit, you may instead give a correct O(n log n)
    // divide-and-conquer algorithm.
}

// TODO
pub fn fn_22() {
    // [7] Consider the problem of examining a string x = x1 x2 . . . xn from an
    // alphabet of k symbols, and a multiplication table over this alphabet.
    // Decide whether or not it is possible to parenthesize x in such a way that
    // the value of the resulting expression is a, where a belongs to the
    // alphabet. The multiplication table is neither commutative or associative,
    // so the order of multiplication matters.
    /*
          | a b c
        --+------
        a | a c c
        b | a a b
        c | c c c
    */
    // For example, consider the above multiplication table and the string
    // bbbba. Paren- thesizing it (b(bb))(ba) gives a, but ((((bb)b)b)a) gives
    // c. Give an algorithm, with time polynomial in n and k, to decide whether
    // such a parenthesization exists for a given string, multiplication table,
    // and goal element.
}

// TODO
pub fn fn_23() {
    // [6] Let α and β be constants. Assume that it costs α to go left in a
    // tree, and β to go right. Devise an algorithm that builds a tree with
    // optimal worst case cost, given keys k1 , . . . , kn and the probabilities
    // that each will be searched p1, . . . , pn .
}

// Interview Problems
// TODO
pub fn fn_24() {
    // [5] Given a set of coin denominators, find the minimum number of coins to
    // make a certain amount of change.
}

pub fn fn_25(data: &[i64]) -> Option<(usize, usize, i64)> {
    // [5] You are given an array of n numbers, each of which may be positive,
    // negative, or zero. Give an efficient algorithm to identify the index
    // positions i and j to the maximum sum of the ith through jth numbers.

    // result = sum(0->j) - sum(0->i), so for result to achieve maximum value,
    // sum(0->j) must be maximum and sum(0->i) is minimum.

    let first = if let Some(v) = data.first() {
        *v
    } else {
        return None;
    };

    let mut sum = first;
    let (mut curmax, mut curmaxat) = (first, 0);
    let (mut curmin, mut curminat) = (first, 0);
    let (mut lstmin, mut lstminat) = (first, 0);
    for i in 1..data.len() {
        sum += data[i];

        // Use '<=' instead of '<' to ignore zeros in data.
        if sum <= curmin {
            curmin = sum;
            curminat = i;
        }

        // Use '>=' instead of '>' so lstmin can be updated faster and more
        // correctly.
        if sum >= curmax {
            curmax = sum;
            curmaxat = i;

            lstmin = curmin;
            lstminat = curminat;
        }
    }

    Some((lstminat, curmaxat + 1, curmax - lstmin + data[lstminat]))
}

// TODO
pub fn fn_26() {
    // [7] Observe that when you cut a character out of a magazine, the
    // character on the reverse side of the page is also removed. Give an
    // algorithm to determine whether you can generate a given string by pasting
    // cutouts from a given magazine. Assume that you are given a function that
    // will identify the character and its position on the reverse side of the
    // page for any given character position.
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_fn_25() {
        assert_eq!(fn_25(&[]), None);
        assert_eq!(fn_25(&[1]), Some((0, 1, 1)));
        assert_eq!(fn_25(&[1, 2, 3]), Some((0, 3, 6)));
    }
}
