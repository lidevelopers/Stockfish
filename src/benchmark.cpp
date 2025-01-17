/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2022 The Stockfish developers (see AUTHORS file)

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

#include "position.h"
#include "uci.h"

using namespace std;
using Stockfish::SUBVARIANT_NB;

namespace {

const vector<string> Defaults[SUBVARIANT_NB] = {
  {
  "setoption name UCI_Variant value chess",
  "setoption name UCI_Chess960 value false",
  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
  "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 10",
  "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 11",
  "4rrk1/pp1n3p/3q2pQ/2p1pb2/2PP4/2P3N1/P2B2PP/4RRK1 b - - 7 19",
  "rq3rk1/ppp2ppp/1bnpb3/3N2B1/3NP3/7P/PPPQ1PP1/2KR3R w - - 7 14 moves d4e6",
  "r1bq1r1k/1pp1n1pp/1p1p4/4p2Q/4Pp2/1BNP4/PPP2PPP/3R1RK1 w - - 2 14 moves g2g4",
  "r3r1k1/2p2ppp/p1p1bn2/8/1q2P3/2NPQN2/PPP3PP/R4RK1 b - - 2 15",
  "r1bbk1nr/pp3p1p/2n5/1N4p1/2Np1B2/8/PPP2PPP/2KR1B1R w kq - 0 13",
  "r1bq1rk1/ppp1nppp/4n3/3p3Q/3P4/1BP1B3/PP1N2PP/R4RK1 w - - 1 16",
  "4r1k1/r1q2ppp/ppp2n2/4P3/5Rb1/1N1BQ3/PPP3PP/R5K1 w - - 1 17",
  "2rqkb1r/ppp2p2/2npb1p1/1N1Nn2p/2P1PP2/8/PP2B1PP/R1BQK2R b KQ - 0 11",
  "r1bq1r1k/b1p1npp1/p2p3p/1p6/3PP3/1B2NN2/PP3PPP/R2Q1RK1 w - - 1 16",
  "3r1rk1/p5pp/bpp1pp2/8/q1PP1P2/b3P3/P2NQRPP/1R2B1K1 b - - 6 22",
  "r1q2rk1/2p1bppp/2Pp4/p6b/Q1PNp3/4B3/PP1R1PPP/2K4R w - - 2 18",
  "4k2r/1pb2ppp/1p2p3/1R1p4/3P4/2r1PN2/P4PPP/1R4K1 b - - 3 22",
  "3q2k1/pb3p1p/4pbp1/2r5/PpN2N2/1P2P2P/5PP1/Q2R2K1 b - - 4 26",
  "6k1/6p1/6Pp/ppp5/3pn2P/1P3K2/1PP2P2/3N4 b - - 0 1",
  "3b4/5kp1/1p1p1p1p/pP1PpP1P/P1P1P3/3KN3/8/8 w - - 0 1",
  "2K5/p7/7P/5pR1/8/5k2/r7/8 w - - 0 1 moves g5g6 f3e3 g6g5 e3f3",
  "8/6pk/1p6/8/PP3p1p/5P2/4KP1q/3Q4 w - - 0 1",
  "7k/3p2pp/4q3/8/4Q3/5Kp1/P6b/8 w - - 0 1",
  "8/2p5/8/2kPKp1p/2p4P/2P5/3P4/8 w - - 0 1",
  "8/1p3pp1/7p/5P1P/2k3P1/8/2K2P2/8 w - - 0 1",
  "8/pp2r1k1/2p1p3/3pP2p/1P1P1P1P/P5KR/8/8 w - - 0 1",
  "8/3p4/p1bk3p/Pp6/1Kp1PpPp/2P2P1P/2P5/5B2 b - - 0 1",
  "5k2/7R/4P2p/5K2/p1r2P1p/8/8/8 b - - 0 1",
  "6k1/6p1/P6p/r1N5/5p2/7P/1b3PP1/4R1K1 w - - 0 1",
  "1r3k2/4q3/2Pp3b/3Bp3/2Q2p2/1p1P2P1/1P2KP2/3N4 w - - 0 1",
  "6k1/4pp1p/3p2p1/P1pPb3/R7/1r2P1PP/3B1P2/6K1 w - - 0 1",
  "8/3p3B/5p2/5P2/p7/PP5b/k7/6K1 w - - 0 1",
  "5rk1/q6p/2p3bR/1pPp1rP1/1P1Pp3/P3B1Q1/1K3P2/R7 w - - 93 90",
  "4rrk1/1p1nq3/p7/2p1P1pp/3P2bp/3Q1Bn1/PPPB4/1K2R1NR w - - 40 21",
  "r3k2r/3nnpbp/q2pp1p1/p7/Pp1PPPP1/4BNN1/1P5P/R2Q1RK1 w kq - 0 16",
  "3Qb1k1/1r2ppb1/pN1n2q1/Pp1Pp1Pr/4P2p/4BP2/4B1R1/1R5K b - - 11 40",
  "4k3/3q1r2/1N2r1b1/3ppN2/2nPP3/1B1R2n1/2R1Q3/3K4 w - - 5 1",

  // 5-man positions
  "8/8/8/8/5kp1/P7/8/1K1N4 w - - 0 1",     // Kc2 - mate
  "8/8/8/5N2/8/p7/8/2NK3k w - - 0 1",      // Na2 - mate
  "8/3k4/8/8/8/4B3/4KB2/2B5 w - - 0 1",    // draw

  // 6-man positions
  "8/8/1P6/5pr1/8/4R3/7k/2K5 w - - 0 1",   // Re5 - mate
  "8/2p4P/8/kr6/6R1/8/8/1K6 w - - 0 1",    // Ka2 - mate
  "8/8/3P3k/8/1p6/8/1P6/1K3n2 b - - 0 1",  // Nd2 - draw

  // 7-man positions
  "8/R7/2q5/8/6k1/8/1P5p/K6R w - - 0 124", // Draw

  // Mate and stalemate positions
  "6k1/3b3r/1p1p4/p1n2p2/1PPNpP1q/P3Q1p1/1R1RB1P1/5K2 b - - 0 1",
  "r2r1n2/pp2bk2/2p1p2p/3q4/3PN1QP/2P3R1/P4PP1/5RK1 w - - 0 1",
  "8/8/8/8/8/6k1/6p1/6K1 w - -",
  "7k/7P/6K1/8/3B4/8/8/8 b - -",

  // Chess 960
  "setoption name UCI_Variant value chess",
  "setoption name UCI_Chess960 value true",
  "bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w HFhf - 0 1 moves g2g3 d7d5 d2d4 c8h3 c1g5 e8d6 g5e7 f7f6",
  "nqbnrkrb/pppppppp/8/8/8/8/PPPPPPPP/NQBNRKRB w KQkq - 0 1",
  "setoption name UCI_Chess960 value false"
  },
#ifdef ANTI
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1",
    "rn1qkbnr/p1pBpppp/8/8/8/4P3/PPPP1PbP/RNBQK1NR w - - 0 4",
    "rnbqkbnr/p2p1ppp/8/2p1p3/2pP4/2N3P1/PP2PP1P/R1BQKBNR b - d3 0 5",
    "rnb1kbn1/p4ppr/8/3q4/1p6/6PB/P3PP1P/R1B1K1NR b - - 1 11",
    "r7/4rp1p/4p3/p1p5/P1P1P2P/8/2K1NPP1/7R w - - 11 26",
    "8/8/1r6/p7/P2k4/8/2pp2K1/3k2K1 w - - 4 42",

    // 2-man positions
    "2K5/8/4r3/8/8/8/8/8 b - - 0 1", // win

    // 3-man positions
    "8/3nP3/8/8/8/8/7R/8 w - - 0 1", // e8B - win
    "8/8/8/8/3K4/5K2/8/1r6 w - - 0 1", // draw

    // 5-man positions
    "8/pb4Pp/8/2p5/8/8/8/8 b - - 0 1", // Bc6 - draw
    "k7/8/3PN3/p4K2/8/8/8/8 b - - 0 1", // loss

    // 6-man positions
    "8/3K4/2K5/3K1K2/8/8/3kk3/8 b - - 0 1", // draw

    // Win by having no pieces left
    "3q4/3k1pp1/7n/5p2/8/4r3/8/8 w - - 0 1",

    // Stalemate
    "1r6/5k2/8/p4b2/P1p5/8/8/8 w - - 0 1", // less pieces
    "7b/7P/8/2p5/2P5/8/8/8 w - - 0 1", // equal number of pieces
    "6bB/5pP1/5P2/8/8/8/8/8 w - - 0 1" // more pieces
  },
#endif
#ifdef ATOMIC
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "rnb1k1nr/pppp1ppp/4pq2/2b5/3P4/2P1PN2/PP3PPP/RNBQKB1R b KQkq - 0 4",
    "r2qk2r/ppQ1p2p/2p1bn2/2np4/8/2N1PP2/PPPP2PP/R1B1K2R b KQkq - 2 12",
    "7r/1p3k1p/4p3/5pp1/3P1P2/1Pn1P3/2P3PP/R5K1 w - - 3 16",
    "2k2r2/p3p2p/5r2/1p1p1n2/8/3PP3/PPPB2PP/R3KR2 w Q - 5 17",
    "3k2n1/r5br/pppppppp/8/PPPPPPPP/8/3NQ3/R1B1KB1R b KQ - 0 16",
    "2k5/p6p/8/1p2p3/PP1p4/4P3/2P2rPP/K3R3 b - - 0 1",
    "5r2/p6k/6p1/1p1p2Bp/8/7P/PPP3P1/5R1K w - - 1 1",
    "6k1/1pp3pp/5p2/8/8/2P1P3/5PPP/6K1 w - - 0 1",
    "8/k7/P7/8/8/6p1/2p3N1/2K5 w - - 0 1"
  },
#endif
#ifdef CRAZYHOUSE
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR[] w KQkq - 0 1",
    "rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR[] w KQkq d6 4 3", // en passant
    "r1bk3r/pppp1Bpp/2n5/4p1N1/4P3/3P4/PPP1p1PP/RNK4R[QBNPqb] b - - 23 12", // repetition detection
    "r3k2r/pppb1ppp/4n3/2P1Q3/2p1n3/2Pb1N2/PP1NpPPP/R1BqR1K1[BP] w kq - 28 15",
    "r1b1kb1r/p1p3pp/2pp4/8/4P3/2NR3P/PPP2P1P/5K1R[QBBNqnnppp] b kq - 39 20", // many pieces in hand
    "r1b1r1k1/ppp1Pppp/8/3p4/3P2P1/PN6/2PBQP1P/q1q~1KB1R[NNrbnp] w - - 42 22", // promoted queen
    "r3kb1r/1bpppppp/p1N2p2/2NPP3/2PP4/2N2Pb1/P3P1R1/R1Q2KBq[Pn] w kq - 54 28",
    "7k/Q2P1pp1/2PPpn1p/3p1b2/3P4/P1n1P3/P1n1bPPP/R1B3KR[RRqbnp] w - - 48 25", // promotion
    "r5k1/p4ppp/4p3/3p2qn/3PB3/4P1B1/PrP2PPP/R4RK1[PBQPBnpnn] w - - 42 22", // syuuban
    "r4r1k/ppp1Nppp/3p1n2/4p3/2B1P3/2PP4/PP3PPP/R1BR2K1[BQNnqb] b - - 0 15", // dynamic middlegame

    // Checkmate
    "r1b2rk1/pppp1ppp/2P2b2/1N6/5N2/4P1K1/P1P4P/1Nrb1b1n~[QQNPPrp] w - - 64 33", // promoted knight
    "1Rbk3r/p1pQ1ppp/2Bn3n/4p1b1/4Pn2/3p4/PbPP1PPP/3RK1R1[QPPn] b - - 45 23",

    // Stalemate
    "2R5/3Q2pk/2p1p3/1pP1P1Qp/1P3P1P/p1P1B3/P7/1R2K3[RRBBBNNNNPPP] b - - 98 55",
  },
#endif
#ifdef EXTINCTION
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
  },
#endif
#ifdef GRID
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r1b1kbnr/pppp1ppp/n7/3NN3/4P3/8/PPPP1qPP/R1BQKB1R b KQkq - 0 1 moves f2h4"
  },
#endif
#ifdef HORDE
  {
    "rnbqkbnr/pppppppp/8/1PP2PP1/PPPPPPPP/PPPPPPPP/PPPPPPPP/PPPPPPPP w kq - 0 1",
    "pppppppp/pppppppp/pppppppp/pppppppp/1pp2pp1/8/PPPPPPPP/RNBQKBNR w KQ - 0 1",
    "rnb3r1/1p1p1k2/pPpP1P2/P1PPPPP1/1PP1PP2/PPP5/PPPP1P1P/PPP3qP w - - 0 1",
    "rn2qk2/1b1p1pbr/3P2n1/pPP1P1Pp/P1PPPP1P/PPPPPPPP/PPP1PPPP/PP4PP w - - 1 22",
    "2b1k2r/2P1n1p1/1P2P1Pp/2PP3P/3PP1PP/4PPPP/5PPP/1q4P1 b k - 0 34"
  },
#endif
#ifdef KOTH
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 10",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 11",
    "4rrk1/pp1n3p/3q2pQ/2p1pb2/2PP4/2P3N1/P2B2PP/4RRK1 b - - 7 19",
    "rq3rk1/ppp2ppp/1bnpb3/3N2B1/3NP3/7P/PPPQ1PP1/2KR3R w - - 7 14 moves d4e6",
    "r1bq1r1k/1pp1n1pp/1p1p4/4p2Q/4Pp2/1BNP4/PPP2PPP/3R1RK1 w - - 2 14 moves g2g4",
    "r3r1k1/2p2ppp/p1p1bn2/8/1q2P3/2NPQN2/PPP3PP/R4RK1 b - - 2 15",
    "r1bbk1nr/pp3p1p/2n5/1N4p1/2Np1B2/8/PPP2PPP/2KR1B1R w kq - 0 13",
    "r1bq1rk1/ppp1nppp/4n3/3p3Q/3P4/1BP1B3/PP1N2PP/R4RK1 w - - 1 16",
    "4r1k1/r1q2ppp/ppp2n2/4P3/5Rb1/1N1BQ3/PPP3PP/R5K1 w - - 1 17",
    "2rqkb1r/ppp2p2/2npb1p1/1N1Nn2p/2P1PP2/8/PP2B1PP/R1BQK2R b KQ - 0 11",
    "r1bq1r1k/b1p1npp1/p2p3p/1p6/3PP3/1B2NN2/PP3PPP/R2Q1RK1 w - - 1 16",
    "3r1rk1/p5pp/bpp1pp2/8/q1PP1P2/b3P3/P2NQRPP/1R2B1K1 b - - 6 22",
    "r1q2rk1/2p1bppp/2Pp4/p6b/Q1PNp3/4B3/PP1R1PPP/2K4R w - - 2 18",
    "4k2r/1pb2ppp/1p2p3/1R1p4/3P4/2r1PN2/P4PPP/1R4K1 b - - 3 22",
    "3q2k1/pb3p1p/4pbp1/2r5/PpN2N2/1P2P2P/5PP1/Q2R2K1 b - - 4 26",
    "6k1/6p1/6Pp/ppp5/3pn2P/1P3K2/1PP2P2/3N4 b - - 0 1",
    "3b4/5kp1/1p1p1p1p/pP1PpP1P/P1P1P3/3KN3/8/8 w - - 0 1",
    "2K5/p7/7P/5pR1/8/5k2/r7/8 w - - 0 1 moves g5g6 f3e3 g6g5 e3f3",
    "8/6pk/1p6/8/PP3p1p/5P2/4KP1q/3Q4 w - - 0 1",
    "7k/3p2pp/4q3/8/4Q3/5Kp1/P6b/8 w - - 0 1",
    "8/2p5/8/2kPKp1p/2p4P/2P5/3P4/8 w - - 0 1",
    "8/1p3pp1/7p/5P1P/2k3P1/8/2K2P2/8 w - - 0 1",
    "8/pp2r1k1/2p1p3/3pP2p/1P1P1P1P/P5KR/8/8 w - - 0 1",
    "8/3p4/p1bk3p/Pp6/1Kp1PpPp/2P2P1P/2P5/5B2 b - - 0 1",
    "5k2/7R/4P2p/5K2/p1r2P1p/8/8/8 b - - 0 1",
    "6k1/6p1/P6p/r1N5/5p2/7P/1b3PP1/4R1K1 w - - 0 1",
    "1r3k2/4q3/2Pp3b/3Bp3/2Q2p2/1p1P2P1/1P2KP2/3N4 w - - 0 1",
    "6k1/4pp1p/3p2p1/P1pPb3/R7/1r2P1PP/3B1P2/6K1 w - - 0 1",
    "8/3p3B/5p2/5P2/p7/PP5b/k7/6K1 w - - 0 1",

    // 5-man positions
    "8/8/8/8/5kp1/P7/8/1K1N4 w - - 0 1",
    "8/8/8/5N2/8/p7/8/2NK3k w - - 0 1",
    "8/3k4/8/8/8/4B3/4KB2/2B5 w - - 0 1",

    // 6-man positions
    "8/8/1P6/5pr1/8/4R3/7k/2K5 w - - 0 1",
    "8/2p4P/8/kr6/6R1/8/8/1K6 w - - 0 1",
    "8/8/3P3k/8/1p6/8/1P6/1K3n2 b - - 0 1",

    // 7-man positions
    "8/R7/2q5/8/6k1/8/1P5p/K6R w - - 0 124",

    // Mate and stalemate positions
    "6k1/3b3r/1p1p4/p1n2p2/1PPNpP1q/P3Q1p1/1R1RB1P1/5K2 b - - 0 1",
    "r2r1n2/pp2bk2/2p1p2p/3q4/3PN1QP/2P3R1/P4PP1/5RK1 w - - 0 1",
    "8/8/8/8/8/6k1/6p1/6K1 w - -",
    "5k2/5P2/5K2/8/8/8/8/8 b - -",
    "8/8/8/8/8/4k3/4p3/4K3 w - -",
    "8/8/8/8/8/5K2/8/3Q1k2 b - -",
    "7k/7P/6K1/8/3B4/8/8/8 b - -",

    // King in the center
    "8/2p5/8/2kPKp1p/2p4P/2P5/3P4/8 b - -",
    "8/8/8/4k3/8/3K4/8/8 w - -"
  },
#endif
#ifdef LOSERS
  {
    "8/4P2p/2pk2p1/1p6/1B2P3/7N/3NKPPP/5B1R b - - 0 22",
    "8/4P2p/2pk2p1/pp6/1B2P3/7N/3NKPPP/5B1R b - - 0 22",
    "1k6/1b2p3/8/3P4/8/8/8/1R5K b - - 0 1",
    "4k3/4p3/8/1r1P3K/B7/8/8/8 b - - 0 1",
    "rnbq1bnr/pppp1kpp/5p2/2P1p3/2B5/K3P3/PP1P1PPP/RNBQ2NR b - - 0 1",
    "1nbq1bnr/1ppkpppp/3p4/1r3P1K/p7/2P5/PP1PP1PP/RNBQ1BNR w - - 0 1"
  },
#endif
#ifdef RACE
  {
    "8/8/8/8/8/8/krbnNBRK/qrbnNBRQ w - - 0 1",
    "8/8/8/2r5/3N1K2/7Q/kqNn2B1/1r1nR3 b - - 4 8",
    "8/6R1/qr3Q2/1k6/4bK2/8/1n1nN1B1/1r2N1R1 w - - 0 9",
    "8/k5K1/1r4Q1/2n2R2/8/8/6B1/8 w - - 4 16",
    "1r6/3R4/1k2N2q/2N1K3/5R2/8/8/8 b - - 11 20"
  },
#endif
#ifdef THREECHECK
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 3+3 0 1",
    "r1bqkb1r/ppp1pppp/2n2n2/3p4/Q7/2P2N2/PP1PPPPP/RNB1KB1R w KQkq - 3+3 0 1"
  },
#endif
#ifdef TWOKINGS
  {
    "rnbqkknr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKKNR w KQkq - 0 1",
    "r1b1kk1r/pp1ppppp/2N2n2/8/4P3/2N5/PPP2qPP/R1BQKK1R w KQkq - 0 7",
    "rnbqk2r/ppppkppp/5n2/4p3/4P3/5N2/PPPPKPPP/RNBQK2R w KQ - 4 4"
  },
#endif
#ifdef ANTIHELPMATE
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
  },
#endif
#ifdef HELPMATE
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "R7/8/8/8/8/8/7k/5K2 b - - 0 1",
    "8/8/8/8/k7/2R5/p5N1/7K b - - 0 1"
  },
#endif
#ifdef GIVEAWAY
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
  },
#endif
#ifdef SUICIDE
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1",
    "rn1qkbnr/p1pBpppp/8/8/8/4P3/PPPP1PbP/RNBQK1NR w - - 0 4",
    "rnbqkbnr/p2p1ppp/8/2p1p3/2pP4/2N3P1/PP2PP1P/R1BQKBNR b - d3 0 5",
    "rnb1kbn1/p4ppr/8/3q4/1p6/6PB/P3PP1P/R1B1K1NR b - - 1 11",
    "r7/4rp1p/4p3/p1p5/P1P1P2P/8/2K1NPP1/7R w - - 11 26",
    "8/8/1r6/p7/P2k4/8/2pp2K1/3k2K1 w - - 4 42",

    // 2-man positions
    "2K5/8/4r3/8/8/8/8/8 b - - 0 1", // win

    // 3-man positions
    "8/3nP3/8/8/8/8/7R/8 w - - 0 1", // e8B - win
    "8/8/8/8/3K4/5K2/8/1r6 w - - 0 1", // draw

    // 5-man positions
    "8/pb4Pp/8/2p5/8/8/8/8 b - - 0 1", // Bc6 - draw
    "k7/8/3PN3/p4K2/8/8/8/8 b - - 0 1", // loss

    // 6-man positions
    "8/3K4/2K5/3K1K2/8/8/3kk3/8 b - - 0 1", // draw

    // Win by having no pieces left
    "3q4/3k1pp1/7n/5p2/8/4r3/8/8 w - - 0 1",

    // Stalemate
    "1r6/5k2/8/p4b2/P1p5/8/8/8 w - - 0 1", // less pieces
    "7b/7P/8/2p5/2P5/8/8/8 w - - 0 1", // equal number of pieces
    "6bB/5pP1/5P2/8/8/8/8/8 w - - 0 1" // more pieces
  },
#endif
#ifdef BUGHOUSE
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR[] w KQkq - 0 1",
    "rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR[] w KQkq d6 4 3", // en passant
    "r1bk3r/pppp1Bpp/2n5/4p1N1/4P3/3P4/PPP1p1PP/RNK4R[QBNPqb] b - - 23 12", // repetition detection
    "r3k2r/pppb1ppp/4n3/2P1Q3/2p1n3/2Pb1N2/PP1NpPPP/R1BqR1K1[BP] w kq - 28 15",
    "r1b1kb1r/p1p3pp/2pp4/8/4P3/2NR3P/PPP2P1P/5K1R[QBBNqnnppp] b kq - 39 20", // many pieces in hand
    "r1b1r1k1/ppp1Pppp/8/3p4/3P2P1/PN6/2PBQP1P/q1q~1KB1R[NNrbnp] w - - 42 22", // promoted queen
    "r3kb1r/1bpppppp/p1N2p2/2NPP3/2PP4/2N2Pb1/P3P1R1/R1Q2KBq[Pn] w kq - 54 28",
    "7k/Q2P1pp1/2PPpn1p/3p1b2/3P4/P1n1P3/P1n1bPPP/R1B3KR[RRqbnp] w - - 48 25", // promotion

    // Checkmate
    "r1b2rk1/pppp1ppp/2P2b2/1N6/5N2/4P1K1/P1P4P/1Nrb1b1n~[QQNPPrp] w - - 64 33", // promoted knight
    "1Rbk3r/p1pQ1ppp/2Bn3n/4p1b1/4Pn2/3p4/PbPP1PPP/3RK1R1[QPPn] b - - 45 23",

    // Stalemate
    "2R5/3Q2pk/2p1p3/1pP1P1Qp/1P3P1P/p1P1B3/P7/1R2K3[RRBBBNNNNPPP] b - - 98 55",
  },
#endif
#ifdef DISPLACEDGRID
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
  },
#endif
#ifdef LOOP
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR[] w KQkq - 0 1",
    "rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR[] w KQkq d6 4 3", // en passant
    "r1bk3r/pppp1Bpp/2n5/4p1N1/4P3/3P4/PPP1p1PP/RNK4R[QBNPqb] b - - 23 12", // repetition detection
    "r3k2r/pppb1ppp/4n3/2P1Q3/2p1n3/2Pb1N2/PP1NpPPP/R1BqR1K1[BP] w kq - 28 15",
    "r1b1kb1r/p1p3pp/2pp4/8/4P3/2NR3P/PPP2P1P/5K1R[QBBNqnnppp] b kq - 39 20", // many pieces in hand
    "r1b1r1k1/ppp1Pppp/8/3p4/3P2P1/PN6/2PBQP1P/q1q1KB1R[NNrbnp] w - - 42 22", // promoted queen
    "r3kb1r/1bpppppp/p1N2p2/2NPP3/2PP4/2N2Pb1/P3P1R1/R1Q2KBq[Pn] w kq - 54 28",
    "7k/Q2P1pp1/2PPpn1p/3p1b2/3P4/P1n1P3/P1n1bPPP/R1B3KR[RRqbnp] w - - 48 25", // promotion

    // Checkmate
    "r1b2rk1/pppp1ppp/2P2b2/1N6/5N2/4P1K1/P1P4P/1Nrb1b1n[QQNPPrp] w - - 64 33", // promoted knight
    "1Rbk3r/p1pQ1ppp/2Bn3n/4p1b1/4Pn2/3p4/PbPP1PPP/3RK1R1[QPPn] b - - 45 23",

    // Stalemate
    "2R5/3Q2pk/2p1p3/1pP1P1Qp/1P3P1P/p1P1B3/P7/1R2K3[RRBBBNNNNPPP] b - - 98 55",
  },
#endif
#ifdef PLACEMENT
  {
    "8/pppppppp/8/8/8/8/PPPPPPPP/8[KQRRBBNNkqrrbbnn] w - -",
  },
#endif
#ifdef KNIGHTRELAY
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
  },
#endif
#ifdef RELAY
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
  },
#endif
#ifdef SLIPPEDGRID
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
  },
#endif
#ifdef TWOKINGSSYMMETRIC
  {
    "rnbqkknr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKKNR w KQkq - 0 1",
    "r1b1kk1r/pp1ppppp/2N2n2/8/4P3/2N5/PPP2qPP/R1BQKK1R w KQkq - 0 7",
    "rnbqk2r/ppppkppp/5n2/4p3/4P3/5N2/PPPPKPPP/RNBQK2R w KQkq - 4 4"
  },
#endif
};

} // namespace

namespace Stockfish {

/// setup_bench() builds a list of UCI commands to be run by bench. There
/// are five parameters: TT size in MB, number of search threads that
/// should be used, the limit value spent for each position, a file name
/// where to look for positions in FEN format, the type of the limit:
/// depth, perft, nodes and movetime (in millisecs), and evaluation type
/// mixed (default), classical, NNUE.
///
/// bench -> search default positions up to depth 13
/// bench 64 1 15 -> search default positions up to depth 15 (TT = 64MB)
/// bench 64 4 5000 current movetime -> search current position with 4 threads for 5 sec
/// bench 64 1 100000 default nodes -> search default positions for 100K nodes each
/// bench 16 1 5 default perft -> run a perft 5 on default positions

vector<string> setup_bench(const Position& current, istream& is) {

  vector<string> fens, list;
  string go, token, varname;

  streampos args = is.tellg();
  // Check whether the next token is a variant name
  if ((is >> token) && (std::find(variants.begin(), variants.end(), token) != variants.end() || token == "all"))
  {
      args = is.tellg();
      varname = token;
  }
  else
  {
      is.seekg(args);
      varname = string(Options["UCI_Variant"]);
  }
  Variant variant = varname == "all" ? CHESS_VARIANT : UCI::variant_from_name(varname);

  do {
  // Assign default values to missing arguments
  string ttSize    = (is >> token) ? token : "16";
  string threads   = (is >> token) ? token : "1";
  string limit     = (is >> token) ? token : (variant == CHESS_VARIANT ? "13" : "12");
  string fenFile   = (is >> token) ? token : "default";
  string limitType = (is >> token) ? token : "depth";
  string evalType = (is >> token) ? token : "mixed";

  go = limitType == "eval" ? "eval" : "go " + limitType + " " + limit;

  if (fenFile == "default")
      fens = Defaults[variant];

  else if (fenFile == "current")
      fens.push_back(current.fen());

  else
  {
      string fen;
      ifstream file(fenFile);

      if (!file.is_open())
      {
          cerr << "Unable to open file " << fenFile << endl;
          exit(EXIT_FAILURE);
      }

      while (getline(file, fen))
          if (!fen.empty())
              fens.push_back(fen);

      file.close();
  }

  list.emplace_back("setoption name Threads value " + threads);
  list.emplace_back("setoption name Hash value " + ttSize);
  list.emplace_back("ucinewgame");
  list.emplace_back("setoption name UCI_Variant value " + variants[variant]);

  size_t posCounter = 0;

  for (const string& fen : fens)
      if (fen.find("setoption") != string::npos)
      {
          list.emplace_back(fen);
          if (fen.find("setoption name UCI_Variant") != string::npos)
              posCounter = 0;
      }
      else
      {
#ifdef USE_NNUE
          if (evalType == "classical" || (evalType == "mixed" && posCounter % 2 == 0))
              list.emplace_back("setoption name Use NNUE value false");
          else if (evalType == "NNUE" || (evalType == "mixed" && posCounter % 2 != 0))
              list.emplace_back("setoption name Use NNUE value true");
#endif
          list.emplace_back("position fen " + fen);
          list.emplace_back(go);
          ++posCounter;
      }
  } while (varname == "all" && ++variant < SUBVARIANT_NB && (is.clear(), is.seekg(args)));

#ifdef USE_NNUE
  list.emplace_back("setoption name Use NNUE value true");
#endif

  return list;
}

} // namespace Stockfish
