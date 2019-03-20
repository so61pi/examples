#include <algorithm>
#include <array>
#include <cassert>
#include <exception>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/operators.hpp>


template<typename ForwardIt>
auto is_element_unique(ForwardIt first, ForwardIt last) -> bool {
    for (; first != last; ++first) {
        if (last != std::find(first + 1, last, *first)) {
            return false;
        }
    }
    return true;
}


template<typename ForwardIt>
auto permutation_parity(ForwardIt first1, ForwardIt last1, ForwardIt first2, ForwardIt last2) -> int {
    assert(std::distance(first1, last1) == std::distance(first2, last2));

    std::vector<bool> mark(std::distance(first1, last1));
    int counter = 0;
    bool done = false;
    do {
        // find next unmark pos
        auto unmark = std::find(mark.begin(), mark.end(), false);
        if (unmark == mark.end()) {
            done = true;
        }
        else {
            auto pos1 = std::next(first1, std::distance(mark.begin(), unmark));
            auto f = pos1;
            bool end_cycle = false;
            do {
                // find next pos in the cycle
                // only accept if it is not marked
                bool found = false;
                auto pos2 = first2;
                long long dis = 0;
                do {
                    pos2 = std::find(pos2, last2, *pos1);
                    if (pos2 == last2) return -1;
                    dis = std::distance(first2, pos2);
                    found = (mark[dis] == false);
                    ++pos2;
                } while (!found);

                pos1 = std::next(first1, dis);
                mark[dis] = true;
                if (*f == *pos1) {
                    end_cycle = true;
                }
                else {
                    ++counter;
                }
            } while (!end_cycle);
        }
    } while (!done);

    return counter;
}


namespace puzzle {

    struct direction {
        enum type {
            begin = 0,
            left = begin,
            top,
            right,
            bottom,
            end
        };
    };


    namespace board {

        // contains size * size numbers
        // 1 special number called blank square
        template<typename Derived>
        class base_board {
            using mytype = base_board;
            using derived_type = Derived;


        protected:
            base_board() = default;
            ~base_board() = default;


        public:
            // slide blank square
            auto slide(direction::type d) -> bool {
                auto&& buffer = this->buffer();
                auto bpos = blankpos();
                auto& row = bpos.first;
                auto& col = bpos.second;

                using std::swap;
                switch (d) {
                case direction::left:
                    if (col == 0)
                        return false;
                    swap(buffer[row][col], buffer[row][col - 1]);
                    --col;
                    blankpos(bpos);
                    break;

                case direction::top:
                    if (row == 0)
                        return false;
                    swap(buffer[row][col], buffer[row - 1][col]);
                    --row;
                    blankpos(bpos);
                    break;

                case direction::right:
                    if (col == size() - 1)
                        return false;
                    swap(buffer[row][col], buffer[row][col + 1]);
                    ++col;
                    blankpos(bpos);
                    break;

                case direction::bottom:
                    if (row == size() - 1)
                        return false;
                    swap(buffer[row][col], buffer[row + 1][col]);
                    ++row;
                    blankpos(bpos);
                    break;

                default:
                    throw std::invalid_argument{ "invalid slide direction" };
                    break;
                }

                return true;
            }


            // find position of a number in board
            // return (-1, -1) if cannot find
            auto find(int num) const noexcept -> std::pair<int, int> {
                auto&& buffer = this->buffer();
                for (auto row = 0; row < size(); ++row) {
                    for (auto col = 0; col < size(); ++col) {
                        if (buffer[row][col] == num) {
                            return std::make_pair(row, col);
                        }
                    }
                }
                return std::make_pair(-1, -1);
            }


            // get & set blank square position
            auto blankpos() const noexcept -> decltype(auto) {
                return static_cast<derived_type const *>(this)->blankpos();
            }


            void blankpos(std::pair<int, int> const& bpos) noexcept {
                static_cast<derived_type*>(this)->blankpos(bpos);
            }


            auto size() const noexcept -> decltype(auto) {
                return static_cast<derived_type const *>(this)->size();
            }


            // read only
            auto operator[](int index) const noexcept -> decltype(auto) {
                return buffer()[index];
            }


            // used by std::map
            auto operator<(mytype const& other) const noexcept -> bool {
                return buffer() < other.buffer();
            }


            auto operator==(mytype const& other) const noexcept -> bool {
                return buffer() == other.buffer();
            }

            auto operator!=(mytype const& other) const noexcept -> bool {
                return !(*this == other);
            }


            // output mytype to ostream
            friend std::ostream& operator<<(std::ostream& os, mytype const& board) {
                auto width = os.width();
                for (auto r = 0; r < board.size(); ++r) {
                    for (auto c = 0; c < board.size(); ++c) {
                        os << std::setw(width) << board.buffer()[r][c];
                    }
                    os << "\n";
                }
                return os;
            }


        private:
            auto buffer() noexcept -> decltype(auto) {
                return static_cast<derived_type *>(this)->buffer();
            }

            auto buffer() const noexcept -> decltype(auto) {
                return static_cast<derived_type const *>(this)->buffer();
            }
        };


        template<int N>
        class aa_board : public base_board<aa_board<N>>
        {
            static_assert(N > 0, "invalid size");

            using mybase = base_board<aa_board<N>>;
            using buffer_type = std::array<std::array<int, N>, N>;

            friend mybase;

        public:
            aa_board() = default;
            aa_board(buffer_type const& buffer, std::pair<int, int> const& blankpos) :
                mybase{},
                m_buffer{ buffer },
                m_brow{ blankpos.first },
                m_bcol{ blankpos.second }
            {
                // check blank square position
                if ((0 <= m_brow && m_brow <= size() - 1) && (0 <= m_brow && m_brow <= size() - 1)) {
                    // then it's OK
                }
                else {
                    throw std::invalid_argument{ "invalid blank position" };
                }
            }


        public: // required by base_board
            auto size() const noexcept -> int {
                return N;
            }


            auto blankpos() const noexcept {
                assert(m_brow != -1 && m_bcol != -1);
                return std::make_pair(m_brow, m_bcol);
            }


            void blankpos(std::pair<int, int> const& bpos) noexcept {
                m_brow = bpos.first;
                m_bcol = bpos.second;
            }


        private: // required by base_board
            auto buffer() noexcept -> buffer_type& {
                return m_buffer;
            }


            auto buffer() const noexcept -> buffer_type const& {
                return m_buffer;
            }


        private:
            buffer_type m_buffer{};
            int m_brow = -1;
            int m_bcol = -1;
        };


        class vv_board : public base_board<vv_board>
        {
            using mybase = base_board<vv_board>;
            using buffer_type = std::vector<std::vector<int>>;

            friend mybase;

        public:
            vv_board() = default;
            vv_board(buffer_type const& buffer, std::pair<int, int> const& blankpos) :
                mybase{},
                m_buffer{ buffer },
                m_brow{ blankpos.first },
                m_bcol{ blankpos.second }
            {
                // check buffer, number of cols must be equal to number of rows
                for (auto& row : m_buffer) {
                    if (static_cast<int>(row.size()) != size()) {
                        throw std::invalid_argument{ "invalid board" };
                    }
                }

                // check blank square position
                if ((0 <= m_brow && m_brow <= size() - 1) && (0 <= m_brow && m_brow <= size() - 1)) {
                    // then it's OK
                }
                else {
                    throw std::invalid_argument{ "invalid blank position" };
                }
            }


        public: // required by base_board
            auto size() const noexcept -> int {
                return static_cast<int>(m_buffer.size());
            }


            auto blankpos() const noexcept {
                assert(m_brow != -1 && m_bcol != -1);
                return std::make_pair(m_brow, m_bcol);
            }


            void blankpos(std::pair<int, int> const& bpos) noexcept {
                m_brow = bpos.first;
                m_bcol = bpos.second;
            }


        private: // required by base_board
            auto buffer() noexcept -> buffer_type& {
                return m_buffer;
            }


            auto buffer() const noexcept -> buffer_type const& {
                return m_buffer;
            }


        private:
            buffer_type m_buffer{};
            int m_brow = -1;
            int m_bcol = -1;
        };


        template<typename Board>
        auto num_of_neighbors(Board const& board) noexcept -> int {
            auto bpos = board.blankpos();
            auto row = bpos.first;
            auto col = bpos.second;

            int non = 0;

            int r1 = row - 1;
            int r2 = row + 1;
            int c1 = col - 1;
            int c2 = col + 1;

            auto is_valid_pos = [&board](int r, int c) -> bool {
                return (0 <= r && r <= board.size() - 1) && (0 <= c && c <= board.size() - 1);
            };

            if (is_valid_pos(r1, c1)) ++non;
            if (is_valid_pos(r1, c2)) ++non;
            if (is_valid_pos(r2, c1)) ++non;
            if (is_valid_pos(r2, c2)) ++non;

            assert(non >= 2);

            return non;
        }


        template<typename Board, typename T>
        auto make_board(T&& buffer, int size, int blank_num = 0) {
            int row = -1;
            int col = -1;
            for (auto r = 0; r < size; ++r) {
                for (auto c = 0; c < size; ++c) {
                    if (buffer[r][c] == blank_num) {
                        row = r;
                        col = c;
                    }
                }
            }

            if (row == -1 || col == -1)
                throw std::invalid_argument{ "cannot find blank square" };

            return Board{ buffer, std::make_pair(row, col) };
        }

    } // namespace board


    namespace graph {

        template<typename Board>
        class neighbor_iterator : public boost::forward_iterator_helper<neighbor_iterator<Board>, Board>
        {
        public:
            neighbor_iterator() = default;
            neighbor_iterator(Board const& board, direction::type d) :
                m_board{ board },
                m_direction{ d }
            {}


            auto operator==(neighbor_iterator const& other) const noexcept -> bool {
                if (m_direction == direction::end && other.m_direction == direction::end) {
                    return true;
                }
                return m_direction == other.m_direction && m_board == other.m_board;
            }


            auto operator++() -> neighbor_iterator& {
                m_direction = static_cast<direction::type>(m_direction + 1);
                return *this;
            }


            auto operator*() const noexcept -> std::pair<Board, Board> {
                auto temp = m_board;
                temp.slide(m_direction);
                return std::make_pair(m_board, temp);
            }


        private:
            Board m_board;
            direction::type m_direction = direction::end;
        };


        /*
        model of
            Graph
            IncidenceGraph
        */
        template<typename Board>
        struct board_graph {
            // Graph concept requirements
            using vertex_descriptor         = Board;
            using edge_descriptor           = std::pair<Board, Board>;
            using directed_category         = boost::undirected_tag;
            using edge_parallel_category    = boost::disallow_parallel_edge_tag;
            using traversal_category        = boost::incidence_graph_tag;

            // IncidenceGraph concept requirements
            using out_edge_iterator         = neighbor_iterator<Board>;
            using degree_size_type          = int;
        };



        // IncidenceGraph concept requirements
        template<typename Board,
            typename Graph = board_graph<Board> // just make the name shorter
        >
        auto out_edges(typename Graph::vertex_descriptor const& board, board_graph<Board> const&)
            -> std::pair<typename Graph::out_edge_iterator, typename Graph::out_edge_iterator>
        {
            return std::make_pair(
                typename Graph::out_edge_iterator{ board, direction::begin },
                typename Graph::out_edge_iterator{ board, direction::end }
            );
        }


        template<typename Board,
            typename Graph = board_graph<Board>
        >
        auto out_degree(typename Graph::vertex_descriptor const& board, board_graph<Board> const&)
            -> typename Graph::degree_size_type
        {
            return num_of_neighbors(board);
        }


        /* boost graph already provides two below functions

        template<typename Board,
            typename Graph = board_graph<Board>
        >
        auto source(typename Graph::edge_descriptor const& e, board_graph<Board> const&)
            -> typename Graph::vertex_descriptor
        {
            return e.first;
        }


        template<typename Board,
            typename Graph = board_graph<Board>
        >
        auto target(typename Graph::edge_descriptor const& e, board_graph<Board> const&)
            -> typename Graph::vertex_descriptor
        {
            return e.second;
        }
        */


        // found exception
        struct found : public std::exception
        {};

        struct exceed_limit : public std::exception
        {};


        template<typename Board>
        class visitor : public boost::default_astar_visitor
        {
        public:
            visitor(Board const& goal, int *counter = nullptr, int vertices_limit = -1) noexcept :
                m_goal{ goal },
                m_counter{ counter },
                m_limit{ vertices_limit }
            {}


            void examine_vertex(Board const& board, board_graph<Board> const&) {
                if (m_limit > 0) --m_limit;
                if (m_counter) ++*m_counter;

                if (board == m_goal)
                    throw found{};

                if (m_limit == 0)
                    throw exceed_limit{};
            }


        private:
            Board const& m_goal;
            int *m_counter = nullptr;
            int m_limit = -1;
        };


        template<typename Board>
        class manhattan_heuristic : public boost::astar_heuristic<board_graph<Board>, int>
        {
        public:
            manhattan_heuristic(Board const& goal) :
                m_goal{ goal }
            {}


            auto operator()(Board const& board) -> int {
                int sum = 0;
                for (auto row = 0; row < board.size(); ++row) {
                    for (auto m_col = 0; m_col < board.size(); ++m_col) {
                        auto pos = m_goal.find(board[row][m_col]);
                        sum += (std::abs(row - pos.first) + std::abs(m_col - pos.second)) << 1;
                    }
                }
                return sum;
            }


        private:
            Board const& m_goal;
        };

    } // namespace graph


    // a map with default value
    template <typename Key, typename Value>
    class default_map {
    public:
        using key_type = Key;
        using data_type = Value;
        using value_type = std::pair<Key, Value>;


        default_map(Value const& defaultValue)
            : defaultValue(defaultValue)
        {}


        auto operator[](Key const& k) -> Value& {
            if (m.find(k) == m.end()) {
                m[k] = defaultValue;
            }
            return m[k];
        }


    private:
        std::map<Key, Value> m;
        Value const defaultValue;
    };

}


// graph_traits partial specialization for board_graph
namespace boost {

    template<typename Board>
    struct graph_traits<puzzle::graph::board_graph<Board>> {
        using G                         = puzzle::graph::board_graph<Board>;

        using vertex_descriptor         = typename G::vertex_descriptor;
        using edge_descriptor           = typename G::edge_descriptor;
        using out_edge_iterator         = typename G::out_edge_iterator;

        using directed_category         = typename G::directed_category;
        using edge_parallel_category    = typename G::edge_parallel_category;
        using traversal_category        = typename G::traversal_category;

        using degree_size_type          = typename G::degree_size_type;

        using in_edge_iterator          = void;
        using vertex_iterator           = void;
        using vertices_size_type        = void;
        using edge_iterator             = void;
        using edges_size_type           = void;
    };

}


namespace puzzle {

    template<typename Board>
    auto check_board(Board const& start, Board const& goal) -> bool {
        // must have same size
        if (start.size() != goal.size()) {
            return false;
        }
        auto const size = start.size();

        // must have same blank number
        auto sbpos = start.blankpos();
        auto gbpos = goal.blankpos();
        if (start[sbpos.first][sbpos.second] != goal[gbpos.first][gbpos.second]) {
            return false;
        }

        std::vector<int> vstart;
        std::vector<int> vgoal;
        vstart.reserve(size * size);
        vgoal.reserve(size * size);
        for (auto r = 0; r < size; ++r) {
            for (auto c = 0; c < size; ++c) {
                vstart.push_back(start[r][c]);
                vgoal.push_back(goal[r][c]);
            }
        }

        // each number can appear only once
        if (!is_element_unique(vstart.begin(), vstart.end()) || !is_element_unique(vgoal.begin(), vgoal.end())) {
            return false;
        }

        // both boards have same set of numbers
        if (!std::is_permutation(vstart.begin(), vstart.end(), vgoal.begin(), vgoal.end())) {
            return false;
        }

        return true;
    }


    template<typename Board>
    auto solvable(Board const& start, Board const& goal) -> bool {
        if (!check_board(start, goal)) {
            throw std::invalid_argument{ "invalid boards, cannot check solvability" };
        }

        auto const size = start.size();

        std::vector<int> vstart;
        std::vector<int> vgoal;
        vstart.reserve(size * size);
        vgoal.reserve(size * size);
        for (auto r = 0; r < size; ++r) {
            for (auto c = 0; c < size; ++c) {
                vstart.push_back(start[r][c]);
                vgoal.push_back(goal[r][c]);
            }
        }

        // compute parity of permutation
        auto parity = permutation_parity(vstart.begin(), vstart.end(), vgoal.begin(), vgoal.end());
        if (parity == -1) {
            return false;
        }

        // compute taxicab distance
        auto sbpos = start.blankpos();
        auto gbpos = goal.blankpos();
        auto tabdis = std::abs(sbpos.first - gbpos.first) + std::abs(sbpos.second - gbpos.second);

        return (parity + tabdis) % 2 == 0;
    }


    template<typename Board>
    auto solve(Board const& start, Board const& goal, int *counter = nullptr, int limit = -1) -> std::vector<Board> {
        using PredecessorMap = boost::associative_property_map<std::map<Board, Board>>;
        using DistanceMap = boost::associative_property_map<puzzle::default_map<Board, int>>;
        using WeightMap = boost::associative_property_map<puzzle::default_map<std::pair<Board, Board>, int>>;
        using VertexIndexMap = boost::associative_property_map<std::map<Board, int>>;
        using RankMap = boost::associative_property_map<std::map<Board, int>>;
        using ColorMap = boost::associative_property_map<std::map<Board, boost::default_color_type>>;


        if (!check_board(start, goal)) {
            throw std::invalid_argument{ "invalid boards" };
        }

        if (!solvable(start, goal)) {
            throw std::runtime_error{ "unsolvable" };
        }

        puzzle::graph::board_graph<Board> graph;

        std::map<Board, Board> pmx{};
        puzzle::default_map<Board, int> dmx{ std::numeric_limits<int>::max() };
        puzzle::default_map<std::pair<Board, Board>, int> wmx{ 1 };
        std::map<Board, int> vimx{};
        std::map<Board, int> rmx{};
        std::map<Board, boost::default_color_type> cmx{};

        PredecessorMap pm{ pmx };

        try {
            boost::astar_search_no_init(
                graph,
                start,
                puzzle::graph::manhattan_heuristic<Board>{ goal },
                boost::visitor(puzzle::graph::visitor<Board>{ goal, counter, limit })
                    .weight_map(WeightMap{ wmx })
                    .vertex_index_map(VertexIndexMap{ vimx })
                    .predecessor_map(pm)
                    .distance_map(DistanceMap{ dmx })
                    .rank_map(RankMap{ rmx })
                    .color_map(ColorMap{ cmx })
                    .distance_compare(std::less<void>{})
                    .distance_combine(std::plus<void>{})
            );
        }
        catch (puzzle::graph::found const&) {
            std::vector<Board> path;

            auto current = goal;
            bool done = false;
            do {
                path.push_back(current);
                if (current == start) {
                    done = true;
                }
                else {
                    current = pm[current];
                }
            } while (!done);

            return path;
        }
        catch (puzzle::graph::exceed_limit const&) {
            // ignore
        }

        return std::vector<Board>{};
    }

} // namespace puzzle


int main() {
    try {
        std::array<std::array<int, 4>, 4> g{ {
            { 1,  2,  3,  4 },
            { 5,  6,  7,  8 },
            { 9, 10, 11, 12 },
            { 13, 14, 15, 0 }
        } };

        std::array<std::array<int, 4>, 4> s{ {
            { 7, 15,  2, 10 },
            { 3,  0,  8,  5 },
            { 6, 11, 13,  1 },
            { 14, 12, 4,  9 }
        } };

        auto goal = puzzle::board::make_board<puzzle::board::aa_board<4>>(g, 4);
        auto start = puzzle::board::make_board<puzzle::board::aa_board<4>>(s, 4);

        int counter = 0;
        auto path = puzzle::solve(start, goal, &counter);

        std::cout
            << "number of tests: " << counter << "\n"
            << "number of steps: " << path.size() - 1 << "\n\n";

        std::cout
            << "start puzzle:\n" << std::setw(3) << start << "\n\n"
            << "goal puzzle:\n" << std::setw(3) << goal << "\n\n";

        // print all intermediate blank square positions
        for (auto it = path.rbegin(), eit = path.rend(); it != eit; ++it) {
            auto pos = it->blankpos();
            std::cout << "(" << pos.first << "," << pos.second << ") -> ";
        }
        std::cout << "done\n";
    }
    catch (std::exception const& e) {
        std::cout << e.what();
    }
}
