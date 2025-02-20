// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_NODE_COINSTATS_H
#define BITCOIN_NODE_COINSTATS_H

#include <chain.h>
#include <coins.h>
#include <consensus/amount.h>
#include <primitives/blockhash.h>
#include <streams.h>
#include <uint256.h>

#include <cstdint>
#include <functional>

class CCoinsView;
namespace node {
class BlockManager;
} // namespace node

namespace node {
enum class CoinStatsHashType {
    HASH_SERIALIZED,
    MUHASH,
    NONE,
};

struct CCoinsStats {
    CoinStatsHashType m_hash_type;
    int nHeight{0};
    BlockHash hashBlock{};
    uint64_t nTransactions{0};
    uint64_t nTransactionOutputs{0};
    uint64_t nBogoSize{0};
    uint256 hashSerialized{};
    uint64_t nDiskSize{0};
    Amount nTotalAmount{Amount::zero()};

    //! The number of coins contained.
    uint64_t coins_count{0};

    //! Signals if the coinstatsindex should be used (when available).
    bool index_requested{true};
    //! Signals if the coinstatsindex was used to retrieve the statistics.
    bool index_used{false};

    // Following values are only available from coinstats index
    Amount total_subsidy{Amount::zero()};
    Amount total_unspendable_amount{Amount::zero()};
    Amount total_prevout_spent_amount{Amount::zero()};
    Amount total_new_outputs_ex_coinbase_amount{Amount::zero()};
    Amount total_coinbase_amount{Amount::zero()};
    Amount total_unspendables_genesis_block{Amount::zero()};
    Amount total_unspendables_bip30{Amount::zero()};
    Amount total_unspendables_scripts{Amount::zero()};
    Amount total_unspendables_unclaimed_rewards{Amount::zero()};

    CCoinsStats(CoinStatsHashType hash_type) : m_hash_type(hash_type) {}
};

//! Calculate statistics about the unspent transaction output set
bool GetUTXOStats(CCoinsView *view, node::BlockManager &blockman,
                  CCoinsStats &stats,
                  const std::function<void()> &interruption_point = {},
                  const CBlockIndex *pindex = nullptr);

uint64_t GetBogoSize(const CScript &script_pub_key);

CDataStream TxOutSer(const COutPoint &outpoint, const Coin &coin);
} // namespace node

#endif // BITCOIN_NODE_COINSTATS_H
