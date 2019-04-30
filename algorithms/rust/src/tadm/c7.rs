pub fn fn_01(n: usize) -> Vec<Vec<u64>> {
    // [3] A derangement is a permutation p of {1, . . . , n} such that no item is
    // in its proper position, i.e. pi != i for all 1 ≤ i ≤ n. Write an efficient
    // backtracking program with pruning that constructs all the derangements of n
    // items.

    #[derive(Debug)]
    struct Item {
        value: u64,
        used: bool,
    }

    fn choose(array: &mut [Item], index: usize) -> Vec<Vec<u64>> {
        if index >= array.len() {
            return vec![];
        }

        fn process(array: &mut [Item], index: usize, val: u64) -> impl Iterator<Item = Vec<u64>> {
            let mut vv = if index + 1 < array.len() {
                choose(array, index + 1)
            } else {
                // Inner empty vector is a placeholder.
                vec![vec![]]
            };
            vv.iter_mut().for_each(|v| v.push(val));
            vv.into_iter()
        }

        (0..array.len())
            .filter_map(|i| {
                if i == index || array[i].used {
                    None
                } else {
                    array[i].used = true;
                    let it = process(array, index, array[i].value);
                    array[i].used = false;
                    Some(it)
                }
            })
            .flatten()
            .collect()
    }

    let mut array = (0..n as u64)
        .map(|e| Item {
            value: e,
            used: false,
        })
        .collect::<Vec<_>>();

    let mut vv = choose(&mut array, 0);
    vv.iter_mut().for_each(|v| v.reverse());
    vv
}

// TODO
pub fn fn_02() {
    // [4] Multisets are allowed to have repeated elements. A multiset of n
    // items may thus have fewer than n! distinct permutations. For example, {1,
    // 1, 2, 2} has only six different permutations: {1, 1, 2, 2}, {1, 2, 1, 2},
    // {1, 2, 2, 1}, {2, 1, 1, 2}, {2, 1, 2, 1}, and {2, 2, 1, 1}. Design and
    // implement an efficient algorithm for constructing all permutations of a
    // multiset.
}

// TODO
pub fn fn_03() {
    // [5] Design and implement an algorithm for testing whether two graphs are
    // isomor- phic to each other. The graph isomorphism problem is discussed in
    // Section 16.9 (page 550). With proper pruning, graphs on hundreds of
    // vertices can be tested reliably.
}

// TODO
pub fn fn_04() {
    // [5] Anagrams are rearrangements of the letters of a word or phrase into a
    // different word or phrase. Sometimes the results are quite striking. For
    // example, “MANY VOTED BUSH RETIRED” is an anagram of “TUESDAY NOVEMBER
    // THIRD,” which correctly predicted the result of the 1992 U.S.
    // presidential election. Design and implement an algorithm for finding
    // anagrams using combinatorial search and a dictionary.
}

// TODO
pub fn fn_05() {
    // [8] Design and implement an algorithm for solving the subgraph
    // isomorphism problem. Given graphs G and H, does there exist a subgraph H'
    // of H such that G is isomorphic to H'? How does your program perform on
    // such special cases of subgraph isomorphism as Hamiltonian cycle, clique,
    // independent set, and graph isomorphism?
}

// TODO
pub fn fn_06() {
    // [8] In the turnpike reconstruction problem, you are given n(n − 1)/2
    // distances in sorted order. The problem is to find the positions of n
    // points on the line that give rise to these distances. For example, the
    // distances {1, 2, 3, 4, 5, 6} can be determined by placing the second
    // point 1 unit from the first, the third point 3 from the second, and the
    // fourth point 2 from the third. Design and implement an efficient
    // algorithm to report all solutions to the turnpike reconstruction problem.
    // Exploit additive constraints when possible to minimize the search. With
    // proper pruning, problems with hundreds of points can be solved reliably.
}

// For each of the problems below, either (1) implement a combinatorial search pro-
// gram to solve it optimally for small instance, and/or (2) design and implement
// a simulated annealing heuristic to get reasonable solutions. How well does your
// program perform in practice?

// TODO
pub fn fn_07() {
    // [5] Design and implement an algorithm for solving the bandwidth
    // minimization problem discussed in Section 13.2 (page 398).
}

// TODO
pub fn fn_08() {
    // [5] Design and implement an algorithm for solving the maximum
    // satisfiability prob- lem discussed in Section 14.10 (page 472).
}

// TODO
pub fn fn_09() {
    // [5] Design and implement an algorithm for solving the maximum clique
    // problem discussed in Section 16.1 (page 525).
}

// TODO
pub fn fn_10() {
    // [5] Design and implement an algorithm for solving the minimum vertex
    // coloring problem discussed in Section 16.7 (page 544).
}

// TODO
pub fn fn_11() {
    // [5] Design and implement an algorithm for solving the minimum edge
    // coloring problem discussed in Section 16.8 (page 548).
}

// TODO
pub fn fn_12() {
    // [5] Design and implement an algorithm for solving the minimum feedback vertex
    // set problem discussed in Section 16.11 (page 559).
}

// TODO
pub fn fn_13() {
    // [5] Design and implement an algorithm for solving the set cover problem
    // discussed in Section 18.1 (page 621).
}

// TODO
pub fn fn_14() {
    // [4] Write a function to find all permutations of the letters in a
    // particular string.
}

// TODO
pub fn fn_15() {
    // [4] Implement an efficient algorithm for listing all k-element subsets of
    // n items.
}

// TODO
pub fn fn_16() {
    // [5] An anagram is a rearrangement of the letters in a given string into a
    // sequence of dictionary words, like Steven Skiena into Vainest Knees.
    // Propose an algorithm to construct all the anagrams of a given string.
}

// TODO
pub fn fn_17() {
    // [5] Telephone keypads have letters on each numerical key. Write a program
    // that generates all possible words resulting from translating a given
    // digit sequence (e.g.,
    // 145345) into letters.
}

// TODO
pub fn fn_18() {
    // [7] You start with an empty room and a group of n people waiting outside.
    // At each step, you may either admit one person into the room, or let one
    // out. Can you arrange a sequence of 2^n steps, so that every possible
    // combination of people is achieved exactly once?
}

// TODO
pub fn fn_19() {
    // [4] Use a random number generator (rng04) that generates numbers from {0,
    // 1, 2, 3, 4} with equal probability to write a random number generator
    // that gener- ates numbers from 0 to 7 (rng07) with equal probability. What
    // are expected number of calls to rng04 per call of rng07?
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_fn_01() {
        assert_eq!(fn_01(0), Vec::<Vec<u64>>::new());
        assert_eq!(fn_01(1), Vec::<Vec<u64>>::new());

        // 0 1
        assert_eq!(fn_01(2), vec![vec![1, 0]]);

        // 0 1 2
        assert_eq!(fn_01(3), vec![vec![1, 2, 0], vec![2, 0, 1]]);
    }
}
