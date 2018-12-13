#include <algorithm>
#include <array>
#include <cassert>
#include <iomanip>
#include <iostream>


template<int BlockSize>
class sudoku_board {
public:
    constexpr static int blocksize = BlockSize;
    constexpr static int dimension = blocksize * blocksize;


public:
    sudoku_board(std::array<std::array<int, dimension>, dimension> board) :
        m_board{ board }
    {}


    auto operator[](std::size_t index) noexcept -> decltype(auto) {
        return m_board[index];
    }


    auto operator[](std::size_t index) const noexcept -> decltype(auto) {
        return m_board[index];
    }


    //
    friend std::ostream& operator<<(std::ostream& os, sudoku_board const& board) {
        auto width = os.width();
        for (auto r = 0; r < dimension; ++r) {
            for (auto c = 0; c < dimension; ++c) {
                os << std::setw(width) << static_cast<int>(board[r][c]);
            }
            os << '\n';
        }
        return os;
    }


private:
    std::array<std::array<int, dimension>, dimension> m_board{};
};


template<typename Board>
class checker {
public:
    // check specific row, col and block
    auto check(Board& board, int r, int c) -> bool {
        index_assert(r);
        index_assert(c);
        return row_check(board, r) && col_check(board, c) && block_check(board, r, c);
    }


    // check entire board
    auto valid_check(Board& board) -> bool {
        for (auto r = 0; r < Board::dimension; ++r) {
            for (auto c = 0; c < Board::dimension; ++c) {
                if (!check(board, r, c)) {
                    return false;
                }
            }
        }
        return true;
    }


    auto complete_check(Board& board) -> bool {
        for (auto r = 0; r < Board::dimension; ++r) {
            for (auto c = 0; c < Board::dimension; ++c) {
                if (board[r][c] == 0 || !check(board, r, c)) {
                    return false;
                }
            }
        }
        return true;
    }


    // assert board number, has no effect in Release build
    void num_assert(int num) const noexcept {
        assert(num_check(num) && "invalid number");
    }


    auto num_check(int num) const noexcept -> bool {
        return num == 0 || (1 <= num && num <= 9);
    }


    void index_assert(int index) const noexcept {
        assert((0 <= index && index < Board::dimension) && "invalid index");
    }


private:
    auto row_check(Board& board, int r) -> bool {
        index_assert(r);
        for (auto c = 0; c < Board::dimension; ++c) {
            m_tempr[c] = board[r][c];
            if (!num_check(board[r][c])) {
                return false;
            }
        }
        std::sort(m_tempr.begin(), m_tempr.end());
        auto pos = std::adjacent_find(m_tempr.rbegin(), m_tempr.rend());
        if (pos != m_tempr.rend() && *pos != 0) {
            return false;
        }
        return true;
    }


    auto col_check(Board& board, int c) -> bool {
        index_assert(c);
        for (auto r = 0; r < Board::dimension; ++r) {
            m_tempc[r] = board[r][c];
            if (!num_check(board[r][c])) {
                return false;
            }
        }
        std::sort(m_tempc.begin(), m_tempc.end());
        auto pos = std::adjacent_find(m_tempc.rbegin(), m_tempc.rend());
        if (pos != m_tempc.rend() && *pos != 0) {
            return false;
        }
        return true;
    };


    auto block_check(Board& board, int r, int c) -> bool {
        index_assert(r);
        index_assert(c);
        auto br = (r / Board::blocksize) * Board::blocksize;
        auto er = br + Board::blocksize;
        auto bc = (c / Board::blocksize) * Board::blocksize;
        auto ec = bc + Board::blocksize;
        for (auto ir = br; ir < er; ++ir) {
            for (auto ic = bc; ic < ec; ++ic) {
                m_tempb[(ir - br)*Board::blocksize + (ic - bc)] = board[ir][ic];
                if (!num_check(board[ir][ic])) {
                    return false;
                }
            }
        }
        std::sort(m_tempb.begin(), m_tempb.end());
        auto pos = std::adjacent_find(m_tempb.rbegin(), m_tempb.rend());
        if (pos != m_tempb.rend() && *pos != 0) {
            return false;
        }
        return true;
    };


private:
    std::array<int, Board::dimension> m_tempr{};
    std::array<int, Board::dimension> m_tempc{};
    std::array<int, Board::dimension> m_tempb{};
};


template<typename Board>
class solver {
public:
    solver(Board& board) :
        m_board{ board },
        m_aset{ board }
    {}


    auto solve() -> bool {
        assert(m_checker.valid_check(m_board.base()));
        int r = 0;
        int c = 0;
        if (m_board.next(r, c)) {
            while (true) {
                auto num = m_board.at(r, c).num();
                if (m_aset.choose(r, c, num)) {
                    m_board.at(r, c).num(num);
                    if (!m_board.next(r, c)) {
                        assert(m_checker.complete_check(m_board.base()));
                        return true;
                    }
                }
                else {
                    m_board.at(r, c).num(0);
                    if (!m_board.prev(r, c)) {
                        return false;
                    }
                    else {
                        m_aset.unchoose(r, c, m_board.at(r, c).num());
                    }
                }
            }
            return false;
        }
        else {
            assert(m_checker.complete_check(m_board.base()));
            return true;
        }
    }


private:
    class wrapped_board;
    class available_set;

    wrapped_board m_board;
    available_set m_aset;
    checker<Board> m_checker;
};


template<typename Board>
class solver<Board>::wrapped_board {
private:
    enum class point_state {
        empty,
        fixed,
        filled
    };


public:
    class point_proxy {
    public:
        point_proxy(int& num, point_state& state) :
            m_num{ num },
            m_state{ state }
        {
            m_checker.num_assert(m_num);
        }


        auto num() const noexcept -> int {
            return m_num;
        }


        void num(int num) noexcept {
            m_checker.num_assert(num);
            assert(m_state != point_state::fixed && "cannot change fixed number");
            m_num = num;
            m_state = (m_num == 0) ? point_state::empty : point_state::filled;
        }


    private:
        int& m_num;
        point_state& m_state;
        checker<Board> m_checker;
    };


public:
    wrapped_board(Board& board) :
        m_board{ board }
    {
        for (auto r = 0; r < Board::dimension; ++r) {
            for (auto c = 0; c < Board::dimension; ++c) {
                m_checker.num_assert(m_board[r][c]);
                m_state[r][c] = (m_board[r][c] == 0) ? point_state::empty : point_state::fixed;
            }
        }
    }


    auto at(int r, int c) noexcept -> point_proxy {
        m_checker.index_assert(r);
        m_checker.index_assert(c);
        return point_proxy{ m_board[r][c], m_state[r][c] };
    }


    // go to next available point
    auto next(int& row, int& col) noexcept -> bool {
        m_checker.index_assert(row);
        m_checker.index_assert(col);
        int r = row, c = col;
        for (; r < Board::dimension; ++r) {
            for (; c < Board::dimension; ++c) {
                if (m_state[r][c] == point_state::empty) {
                    row = r, col = c;
                    return true;
                }
            }
            c = 0;
        }
        return false;
    }


    // back to nearest available point
    auto prev(int& row, int& col) noexcept -> bool {
        m_checker.index_assert(row);
        m_checker.index_assert(col);
        int r = row, c = col;
        for (; r >= 0; --r) {
            for (; c >= 0; --c) {
                if (m_state[r][c] == point_state::filled) {
                    row = r, col = c;
                    return true;
                }
            }
            c = Board::dimension - 1;
        }
        return false;
    }


    // return original board
    auto base() noexcept -> Board& {
        return m_board;
    }

    auto base() const noexcept -> Board const& {
        return m_board;
    }


private:
    Board& m_board;
    std::array<std::array<point_state, Board::dimension>, Board::dimension> m_state{};
    checker<Board> m_checker;
};


template<typename Board>
class solver<Board>::available_set {
public:
    available_set(Board const& board) {
        for (auto i = 0; i < Board::dimension; ++i) {
            rows[i][0] = -1;
            cols[i][0] = -1;
            blks[i][0] = -1;
        }

        for (auto r = 0; r < Board::dimension; ++r) {
            for (auto c = 0; c < Board::dimension; ++c) {
                auto num = board[r][c];
                m_checker.num_assert(num);
                if (num != 0) {
                    inc(r, c, num);
                }
            }
        }
    }


    auto choose(int r, int c, int& num) -> bool {
        m_checker.index_assert(r);
        m_checker.index_assert(c);
        m_checker.num_assert(num);
        int b = (r / Board::blocksize) * Board::blocksize + c / Board::blocksize;
        auto& row = rows[r];
        auto& col = cols[c];
        auto& blk = blks[b];

        for (int i = num + 1; i <= Board::dimension; ++i) {
            if ((row[i] | col[i] | blk[i]) == 0) {
                num = i;
                inc(r, c, num);
                return true;
            }
        }

        return false;
    }


    void unchoose(int r, int c, int num) {
        m_checker.index_assert(r);
        m_checker.index_assert(c);
        m_checker.num_assert(num);
        dec(r, c, num);
    }


private:
    void inc(int r, int c, int num) {
        m_checker.index_assert(r);
        m_checker.index_assert(c);
        m_checker.num_assert(num);
        int b = (r / Board::blocksize) * Board::blocksize + c / Board::blocksize;
        ++rows[r][num];
        ++cols[c][num];
        ++blks[b][num];
    }


    void dec(int r, int c, int num) {
        m_checker.index_assert(r);
        m_checker.index_assert(c);
        m_checker.num_assert(num);
        int b = (r / Board::blocksize) * Board::blocksize + c / Board::blocksize;
        --rows[r][num];
        --cols[c][num];
        --blks[b][num];
    }


private:
    std::array<std::array<int, Board::dimension + 1>, Board::dimension> rows{ 0 };
    std::array<std::array<int, Board::dimension + 1>, Board::dimension> cols{ 0 };
    std::array<std::array<int, Board::dimension + 1>, Board::dimension> blks{ 0 };
    checker<Board> m_checker;
};


int main() {
    std::array<std::array<int, 9>, 9> array{ {
        //1  2  3  4  5  6  7  8  9
        { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
        { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
        { 0, 9, 8, 0, 0, 0, 0, 6, 0 },

        { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
        { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
        { 7, 0, 0, 0, 2, 0, 0, 0, 6 },

        { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
        { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
        { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
        } };
    sudoku_board<3> board{ array };
    checker<decltype(board)> checker;
    if (checker.valid_check(board)) {
        solver<decltype(board)> solver{ board };
        if (solver.solve()) {
            std::cout << std::setw(3) << board;
        }
        else {
            std::cout << "cannot solve\n";
        }
    }
    else {
        std::cout << "invalid board\n";
    }
}
