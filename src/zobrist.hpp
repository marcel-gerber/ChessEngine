#ifndef CHESSENGINE_ZOBRIST_HPP
#define CHESSENGINE_ZOBRIST_HPP

#include "piece.hpp"

#include <cstdint>

class Zobrist {

private:
    // Zobrist-Array for Pieces
    // 12 Pieces (6 Piece types * 2 Colors) * 64 Squares
    static constexpr uint64_t Pieces[768] = {
            0xed7aa055607d1a8fULL, 0x939d4d825cac824bULL, 0xc1efabb6378ba7f1ULL, 0xdba718253baa0905ULL,
            0x9afe4d8a9822e1aaULL, 0x36c6797e5cad3f7eULL, 0xc40ad608b6665e7bULL, 0x2b15380a6333e1d3ULL,
            0xbe38e5a9b89042e6ULL, 0xafd79a80f5a15677ULL, 0xd309d15fc7dad47aULL, 0x4fb7529dec07f10fULL,
            0x7ead777a04221958ULL, 0x5f682ab5efe32124ULL, 0xfeb10c91ffdd82bdULL, 0x58acad77010871dbULL,
            0xecbc3d0ee301dbacULL, 0x558ed8c07fb82b29ULL, 0x8f39389cf5963255ULL, 0x16ba543aec4119feULL,
            0x6461370f10aaf703ULL, 0xbe84f1db9c816fbfULL, 0xa043816619e8c3c3ULL, 0x939d3079e57c1785ULL,
            0x04b8a8e1488cfb4dULL, 0x6d23a2646b04386bULL, 0x42e5271453f1af30ULL, 0x74c822b6ce80a878ULL,
            0x886760546515072dULL, 0xe47ce1494e408734ULL, 0x4744e02d0a95f001ULL, 0xde3454b7ac133c22ULL,
            0x09f3d05cf439e0e5ULL, 0xa72e182be827a4bbULL, 0x70219ebef6784e67ULL, 0xabc3a26ba6374334ULL,
            0xeeccc790eaa85d7dULL, 0x313809c628af4f42ULL, 0x36c56e1de87f9356ULL, 0x682484a2fec00136ULL,
            0xc15490e4fa5c70d6ULL, 0x111e41879858b259ULL, 0xc4ec95a36b2940b6ULL, 0xe5613f9cac7a23e0ULL,
            0x9019777d80602cbbULL, 0x273e796910c72d66ULL, 0x8a93aab71576b8e2ULL, 0x9d70f5eeb67caea1ULL,
            0xe5ffd439263b5845ULL, 0xc280823ba2836dacULL, 0x1c03f3af0b076da1ULL, 0x6d7de797440c54e7ULL,
            0x4034f3b71f955d05ULL, 0xd6bc148ec0a21e95ULL, 0x67b81f19780d242bULL, 0x94455183e5d1d459ULL,
            0x1543300c1f554859ULL, 0xa33693a9b73d5285ULL, 0xe066a61c3a3b6516ULL, 0xf36d8a74e2c1f646ULL,
            0x1280594bf2537e4bULL, 0xdff2a0c4a40ab15eULL, 0x8fbcfd66a8f12a56ULL, 0xb96d79fa36f37570ULL,
            0xd2bf8ae66de799beULL, 0x37bdcb0a27f67bb6ULL, 0xf88448d364cff053ULL, 0xe1c1225e575397aaULL,
            0xf3838511242b298eULL, 0x1c1820a739a1b865ULL, 0x0c69ad1f241a3588ULL, 0x43eee4bc9365e909ULL,
            0x31d93be3da47ba67ULL, 0x37fb4706fe333f88ULL, 0x6323db55f14a0311ULL, 0x6a9a685652e68539ULL,
            0xd4402e841582d897ULL, 0xc69e0db5eb032e1bULL, 0x2689742b4a38ae04ULL, 0x25d2d7513df3233fULL,
            0x333a88f921130cd2ULL, 0xeb8246fd6cef0ec4ULL, 0x8f453a37a9d3961dULL, 0x0ddd948b00451360ULL,
            0x7e0edee84c70e469ULL, 0x90ece175eafd2d8fULL, 0x8720d4c5dde07125ULL, 0x124f9464059ba0f5ULL,
            0xeea42cb9f3943682ULL, 0x8429c613f50d709aULL, 0xdffad8c39595c160ULL, 0xc31b1c43cc3ba318ULL,
            0xba133fed06daceb2ULL, 0x52df26c70a772e90ULL, 0x9bbe86e334692413ULL, 0x1cd966d24b045ac3ULL,
            0x3c3b7556679a32feULL, 0xca5d154cc683f708ULL, 0x74b1520f7bc8136eULL, 0x9a7563ca328ae236ULL,
            0x9cf89012a1983637ULL, 0x823f08d4dd10d609ULL, 0x3b5c740ef73df373ULL, 0xdc1d151f14254dd0ULL,
            0x1c4070ea787f336cULL, 0xf6ae9f5243c54804ULL, 0xcc8d9a641859d1adULL, 0xa87716e9d444952dULL,
            0x47554286a2922efcULL, 0x4e62e5cddd655695ULL, 0x05d436679546fe67ULL, 0xd0b2bbf86b915f6cULL,
            0x7bc6cfc6b58b9f5eULL, 0x832db79f726786eaULL, 0x9e85788d5e2f5072ULL, 0xc5433df1d64b712cULL,
            0xd74148f4e7775fcdULL, 0x731415577466660aULL, 0x79967c7efa10073eULL, 0xfceaf82a8ae6e242ULL,
            0x1ffabd438c07aa05ULL, 0x37865d3cf9b070d6ULL, 0x21acc97e45b4e136ULL, 0xcbaaf20a4dc9fd1cULL,
            0xe9d48dc49fab2a99ULL, 0x0f1710100966689cULL, 0xe1596bca2dcf5ba7ULL, 0x5b3652239ba9cf0dULL,
            0xc8ea0bd2bd4a1f21ULL, 0xee6f451efd1e01a0ULL, 0xd7c75cc3c6bd6fdcULL, 0x5cc86a839d813ef3ULL,
            0x4ecff3724a2015d7ULL, 0xaecda0e46cbf38ebULL, 0x44bb80a36bd03999ULL, 0xa2e5c32c61eb6734ULL,
            0x2a3a3bd7c2f338a6ULL, 0x9153b5d264130390ULL, 0x1acbd5e9fe5c30c2ULL, 0xe9320a0fc7a9ff4cULL,
            0x954903777b2ef648ULL, 0x9f6a075e2cd1cf0eULL, 0x5cf6645d3181addcULL, 0xbfccff51be2490f9ULL,
            0x4521a3bcff699329ULL, 0xb8ea8cced001599bULL, 0x3a68eb70d0822f04ULL, 0xea13c01c521e4d2aULL,
            0xc732f4ecfefcc0ceULL, 0x02f4a3aaffd1e43fULL, 0xfa4c057e22eadc34ULL, 0xd784f2f6d777fb4dULL,
            0x6aea2202ac32d22bULL, 0xfc78b91a8231804dULL, 0xeccaaf952f3ca4c5ULL, 0x8b41875ae83cb4d9ULL,
            0x0d467fdf338059d2ULL, 0x2e1ebeb0aaf617eeULL, 0x135fb83c4d2c5e4eULL, 0x8af97318addf4ca6ULL,
            0x7b3ce8dd2a87aeb9ULL, 0xb1ed7f5cbe194f1eULL, 0x49ffb14dfddc90e1ULL, 0x20d61fae7634420cULL,
            0x8ed52b7814e2056bULL, 0x528f25fd1b00b48dULL, 0x89cb5388eb955c6bULL, 0x0f6b2e5743880b1eULL,
            0xcfb781a511a1246eULL, 0xa5b6b572943fdb7fULL, 0x3fdb7c1a1de88a2aULL, 0x0ad6998e15467f4dULL,
            0x9ea4b46c68ae6e5aULL, 0xa4d5bb8219c914f7ULL, 0x3473e88c611d2676ULL, 0x854b7440eed146d0ULL,
            0x78dad224943d5021ULL, 0x1af8e0eb19bf5a6bULL, 0xcd7774490f7f3b51ULL, 0xbe2c6be2250b04cdULL,
            0x3a8a948157bfcae3ULL, 0xb3477ecda6ac3eafULL, 0x1b90b0a4d152ea2aULL, 0xc794e415871914abULL,
            0x28ffcedc250da01aULL, 0xb5e7448c9c32e29eULL, 0xbded6fb513aa2dc9ULL, 0x3dfac70c728e556eULL,
            0xa3dde9e1f384cd52ULL, 0x701faee5ab05a236ULL, 0xb90d6194f8b680b8ULL, 0x4c4f48566cc67007ULL,
            0xde9d4f300674e58aULL, 0x23d88433445c6128ULL, 0x5f4ed09770527df6ULL, 0x7cd9104da70a1889ULL,
            0xe4131b7f93b11070ULL, 0x22dc13db66a5573eULL, 0x4fbe7c4963806eb6ULL, 0xd8f1b49f3ef32360ULL,
            0x703c70d7d6f4e960ULL, 0xd49529cad91452f0ULL, 0x5a40e248211db8f2ULL, 0x3437245536aeceb3ULL,
            0x0fc696c55d0053a7ULL, 0x26464c764bd2d622ULL, 0x25c7ac4734989570ULL, 0x3e961f1a30650309ULL,
            0x8018a9e2081831eeULL, 0x896ac66d78f8edb0ULL, 0x0de7c3acbfbfe536ULL, 0x8966cd27b00b7d2cULL,
            0x5d14bdf90f9021cbULL, 0xfe9e6abff2546cccULL, 0x4e0c84962a4a07eaULL, 0xafb743f7fed04dc9ULL,
            0x22524dfd4a6f5844ULL, 0xb555414257ec95f0ULL, 0x907a4ff87fabbffeULL, 0x90385cbbfc415b79ULL,
            0xa410bceb72a4c2d2ULL, 0xc3837386c87e625cULL, 0x7d194b02110710bcULL, 0xb04ca0bb411e3c6aULL,
            0x0589880be78411a2ULL, 0x5e9118f2be492403ULL, 0x737fe0df91e36c8aULL, 0x7f0b9eb4c3ed2b93ULL,
            0x55ea95a34d7501c1ULL, 0xc472ff4ba1a3ea79ULL, 0xb61742223425405aULL, 0x229777b0494e61b2ULL,
            0x15b042197d9ac56fULL, 0x07ed05cb18b79be3ULL, 0x29ac78e6e5c90b04ULL, 0x30ab5a1c8248e1c2ULL,
            0xaf2ed6a6c19a3ba3ULL, 0xfb42cdd07ecbf023ULL, 0x56837f728bc691d2ULL, 0x8052c3b85a6f4d76ULL,
            0xc1b0ddfa8a82eee4ULL, 0x36aa4536284cdb9cULL, 0xff76ef29a02f1046ULL, 0xab82741d597e5dafULL,
            0x5adfdb0d0a6d7f70ULL, 0x860261a2422a7f74ULL, 0x848fd2d5511b7edcULL, 0x6f569226348895a7ULL,
            0x83a87b6289fadc14ULL, 0x2adcacec04ee9954ULL, 0x165cf226bc15a1a7ULL, 0x51f4c7bad7aefdc5ULL,
            0x8c2ec5e0f065b5b4ULL, 0x5f2393494880cd9aULL, 0xe3a4a5d9c84c1ca2ULL, 0xb2db6d14d1fe7830ULL,
            0xe0912c0dc2793020ULL, 0x29eceb0a86a5fb7dULL, 0xde44155cdaa76dc6ULL, 0xcf6cc4f40655df88ULL,
            0x79c652185fcff478ULL, 0xeb393fbbc444ea5eULL, 0x1a5d042b2b41701eULL, 0x36e8a6bd42c37503ULL,
            0xab43a3f51768a0a2ULL, 0x492d53684cffcc31ULL, 0xe2d9900c414644f1ULL, 0xbc88cd658621c0ccULL,
            0x5db6ead8ac82cfefULL, 0xc363ef81f9825078ULL, 0x9646d8975e4b85d4ULL, 0x19d51e664bd12a4eULL,
            0xd9e498a04043a47dULL, 0x6812262deac4db39ULL, 0x68dec55cb94d7f20ULL, 0x60d99ce0ff8610ccULL,
            0xe281ad00317902deULL, 0x7ec4e206b5984a04ULL, 0x3c323fd64884f456ULL, 0x098267c5c221e1c5ULL,
            0x3435080da9d633ddULL, 0x9ba707302342cef0ULL, 0x8df97eae5dfcc416ULL, 0x0a41ca8da8361bffULL,
            0x34d58039cf39902aULL, 0x3237c2693f0acff5ULL, 0x3bf2a60bcdd381e6ULL, 0xd3179720f78b9b5fULL,
            0x4690b4214a1156e6ULL, 0xa7fd78802e7bdac9ULL, 0x2f8f0ae6d119681dULL, 0x579a870303973d59ULL,
            0x5ed6279ec675f6a3ULL, 0x7127cd527b5c222dULL, 0x21f48f551e3bc309ULL, 0x368ba36828abc22dULL,
            0xa3905b739090d9ccULL, 0x99e676cff0d1ff68ULL, 0xb2d7ebd6b170ce92ULL, 0x4c2c1205f97e8310ULL,
            0x188522f09c2076f7ULL, 0xee596b71f1e47b7aULL, 0x4497f7dd2577ca36ULL, 0x2c6997b5c533f715ULL,
            0xc823d62089e6ea0fULL, 0x6c8b66af29c94399ULL, 0xe11800c8d17b29beULL, 0xf804ea4aa8236304ULL,
            0xd826b9ec4a063883ULL, 0xb178762cb2b8a0dbULL, 0x996b021cee073cdcULL, 0x081f3e11194bfbafULL,
            0xc6d9918b692575b8ULL, 0xcf7b36c4fdb9920dULL, 0x8ef542970b5c4788ULL, 0xdcd78dff0dd2b44eULL,
            0x8ecc43eb72290aeeULL, 0x01ccfac45e6c3607ULL, 0x271146daa1a576f5ULL, 0xa64245e77442fd7dULL,
            0x8efd3828313d9330ULL, 0xe982dc0a238f4901ULL, 0x154c34675fc67aa3ULL, 0xc216a7b11c8a9c77ULL,
            0x7a80405ee5f36497ULL, 0xf798aadc7e253be7ULL, 0x7c12208fe7edced0ULL, 0xcf387a2d22931ce8ULL,
            0x54b6658bbe93fe88ULL, 0x45b161b99c5070fdULL, 0x508ed03eada507d7ULL, 0x4ca5037328fdad47ULL,
            0x807f71126264d81bULL, 0x0075e1866cf55c65ULL, 0x435a4e8de3213696ULL, 0x7efe173079acb30cULL,
            0x9efab79b65f14938ULL, 0xf5e02cc8a38debfcULL, 0xcf483dd198742083ULL, 0x707b4d360a258d0eULL,
            0x3b0196cbaf7a7441ULL, 0xeaabd325dc35408eULL, 0x13a9c7dd0e97c7c0ULL, 0x0b8dd9a8c56a3f6eULL,
            0x06baa0613bac008eULL, 0xd950cf60f1fb7144ULL, 0x3a8d2f88464981ebULL, 0x6cf4eb258630913bULL,
            0xddce4fb234c72c28ULL, 0xbaff6c8a821f8f60ULL, 0xad7d390c2a5e9f63ULL, 0x169861840cdaea71ULL,
            0x00312cf80ca2c9f5ULL, 0x88aa22b08b823a3dULL, 0x6e158bb6300a4b98ULL, 0x37261875171bff15ULL,
            0x46890b8bea5b8c48ULL, 0xc99f1643ce2e3c43ULL, 0x17d3fb6de9a1a9b8ULL, 0x4065a9ca8c086142ULL,
            0xda19c899b130edf5ULL, 0x7129a218440061a6ULL, 0xdc17227258fb206fULL, 0x670a9519db560f83ULL,
            0x645681a5e8c50d05ULL, 0xddff917f3158cbf5ULL, 0x68399e720027b5dcULL, 0xb8591bf10411d75fULL,
            0x7cd3055a720842a0ULL, 0xe7169f733caf11f0ULL, 0xc302609f51c5ceabULL, 0xa808d0d08122c704ULL,
            0xc3aa4217f9739ad1ULL, 0x0043682fec085a71ULL, 0xc21ea7b2ba58e08aULL, 0xa4075b6c9e396d6cULL,
            0x8a441cd49a664f08ULL, 0xafa35723ff18af23ULL, 0x7308d20010bd8c2dULL, 0x6a7a15b8adf404c9ULL,
            0xd04f3602ffc97631ULL, 0xcb2d4f07177bd55fULL, 0x8e4e56a57c212fddULL, 0x71a526b55368f262ULL,
            0x8dcd44fe3d653d52ULL, 0x732b1804f8e70849ULL, 0x7390fccf4e526a6cULL, 0xc43263ef63d49743ULL,
            0x34eb459e6f008e9eULL, 0x57e12f089f86cc71ULL, 0xc5b6c3c8465c5d30ULL, 0x3761b42f92122ba5ULL,
            0xdd39a7162a43447dULL, 0x800e5fd604c13005ULL, 0x7859642a3a606a85ULL, 0x951f6937029aaa36ULL,
            0x844af7a8664e4a5fULL, 0x7a292c330315734cULL, 0xc831f806ce13ab30ULL, 0x9c75e6aca20b0e03ULL,
            0xcec51465f4a92b78ULL, 0x9c1a4821e826c0e6ULL, 0x5426a2c2eb59ff92ULL, 0x95193230853dfd8dULL,
            0x3a8f98b35ace2d08ULL, 0x8ccbe361dcda0e1eULL, 0x031d0259e98b9d14ULL, 0x28a7d46067d2718aULL,
            0xaf30b4f4d72ae914ULL, 0xc0649be059601e29ULL, 0xeab0e5482758fb5fULL, 0x1b8debb3cf855da0ULL,
            0x17624b75624bb913ULL, 0xf1b820230590ab58ULL, 0x408bb485cf3a2fe0ULL, 0xc951379aed4ac4afULL,
            0xd3b580702acee18dULL, 0xe8281b3754630dc3ULL, 0xd7c65fe0c6672d19ULL, 0x07f9533154a10833ULL,
            0x7797a6ce59840685ULL, 0x9a71f638ac9d9610ULL, 0xaa4bf89061b7dd2aULL, 0x922648ceb7964f43ULL,
            0xb809c26bbdf3128bULL, 0xbf2a15bc4e75b0c1ULL, 0x147d59c0e9669ae9ULL, 0x0e9475109114db7eULL,
            0x6990f43232180f14ULL, 0x860dee87fb355f07ULL, 0x9194ab364478b552ULL, 0xbad0991746e74eb0ULL,
            0x720eb3b47dd2a0a0ULL, 0x833f890bea2c8de1ULL, 0x2acab53c526ebcbfULL, 0xee5b856c99e8d0e2ULL,
            0x9144e9d93e9418d2ULL, 0x97c590ff61a76735ULL, 0xa29017b398c35e5aULL, 0x2c8f5b1124d59a27ULL,
            0x02b64c74d82b2abbULL, 0x3b67a041767996a5ULL, 0x2b1aa5fb4f135b33ULL, 0x28932b36ecd2dc26ULL,
            0x09cf7dd85a458183ULL, 0xea6e0eaebef07f0eULL, 0xba0ac2774d58ed3fULL, 0x910e10292f8ff143ULL,
            0xcb91fed8cc7a8f67ULL, 0xb3630976203ff799ULL, 0x1b87bf2a64e377a5ULL, 0xe1be30ba3c7e1e00ULL,
            0xce4c7f32e51412daULL, 0x28d4ed7de51a34d7ULL, 0x9892b3e1d6f5c59eULL, 0xbc508111f868c5cbULL,
            0xadcf1b5731306329ULL, 0xfd63877574b2b0aeULL, 0x36dc5445ae400cb7ULL, 0x34b1051b507be220ULL,
            0x5c3bbd92281fb07cULL, 0x4de8efc4a9246436ULL, 0x6a27e45e808c7351ULL, 0xc543729eb96e7243ULL,
            0xe71b7ba51804e4edULL, 0x221d72f27e9741bfULL, 0x390cc341cc17b338ULL, 0x8a101712481da3deULL,
            0x530dd6a411018c58ULL, 0x5ebcda11e8adec68ULL, 0xb47bb843f2554591ULL, 0xfe3bec681321e4dbULL,
            0xf0b14789de195833ULL, 0xec0f3c7c4896cdcaULL, 0x14bca25b24df8709ULL, 0x099c349f1ddf60f2ULL,
            0x65df908e111f231eULL, 0xe19548169341eae6ULL, 0x971fcc941f0c72feULL, 0x11561a877e9e2db0ULL,
            0xdcfc35f30c5afd34ULL, 0x44f1d159a36342edULL, 0xfc1bd6a7c1b44610ULL, 0x586f50b324bba576ULL,
            0xae1a3a84f17d5cc6ULL, 0x0cd91a3df62667afULL, 0x76f91c84cba27630ULL, 0x64461aa4d5eea884ULL,
            0xb9db34959f31523dULL, 0x6850981ecd4d2ae5ULL, 0xf257b4ac99544c1eULL, 0xb1d9c7033ffa116aULL,
            0x6352fe75193d09f5ULL, 0x113f76d82a0af7feULL, 0xb8ad964f5e2fb52cULL, 0x7f54c5043deea472ULL,
            0x4937c1cc19e7b83cULL, 0x8853709cca8f9b29ULL, 0x12f96ca89a90e679ULL, 0x3e0f643e301fad00ULL,
            0xe2e68765e79c15c4ULL, 0xfc6fcc444db783cdULL, 0x43601012bf3da8fdULL, 0x75d9a7982fec559aULL,
            0xdf64fbd429fb63d2ULL, 0xf556deb92ac833d1ULL, 0x69e56c7eef325d07ULL, 0x7e62cd5085e0f41bULL,
            0x528d16f3735729a5ULL, 0x10af416f31e6be69ULL, 0x599fda2ec34b7e20ULL, 0xcef9e56d08ddd824ULL,
            0x43e08a39ffa00973ULL, 0xdab8f2e63d61ca3eULL, 0x1432f3b7d215ef00ULL, 0x763379592b151f07ULL,
            0x5b610bd435ddeb04ULL, 0x0acc81b76ce391b9ULL, 0x97aed0a8e27e68eaULL, 0xc1c89ca9aebf3cc2ULL,
            0xf4b4dfa419384b69ULL, 0xce8c2a7d0bd1c100ULL, 0x7b676ea2a673e99dULL, 0x8e6a23d52d8fd1fbULL,
            0xbdfe33a06b847d1cULL, 0x04aa20987e5d94c4ULL, 0xd4a07a052e47b2a2ULL, 0x7c8382c40e409c00ULL,
            0xa8cf18ac96bdc6f7ULL, 0xe508b9680f73b730ULL, 0xfadf75b18005ae9dULL, 0xdeb8456b67c25616ULL,
            0xfec77670ef297fceULL, 0xb18631807e9f25ddULL, 0x5deb8f4d959a2ed9ULL, 0xb37b9a8fb32b280bULL,
            0xcb7a5f7bc6cedf01ULL, 0x6c1d9839a24c719dULL, 0x198f504d29b68a18ULL, 0x118dd116622950f4ULL,
            0x97402720a33f6080ULL, 0x82c527db4bd7340bULL, 0xb4196d791d9bd0ddULL, 0x478a97184adba340ULL,
            0x0c0028cb3b36ba49ULL, 0xf08e87a977c9c273ULL, 0x07b6bdf2ba5c41bbULL, 0xdeaced2944e5f702ULL,
            0xa2badc3519cb51eaULL, 0x35224f4a6c9c8988ULL, 0xbb87cdccce9a88f9ULL, 0x6e89780bce2fb003ULL,
            0xdec9145c91ff4168ULL, 0x791d3d2e36debabbULL, 0xd0905b63621597e0ULL, 0xf3fed6431e04c33dULL,
            0x03d485069fe5e172ULL, 0xacb1d11c80e8466cULL, 0xf1d33c735b998511ULL, 0x003f5790bf157f4eULL,
            0x257c3abbeda5f3daULL, 0xbc726f3ebd87bcd2ULL, 0xc75f8fa418ce045eULL, 0x1c1374d85cac594aULL,
            0xceec1d9f9462e64cULL, 0xe0cfce6ee910a08dULL, 0x5556eab04633ba5bULL, 0x2391afefdb18120dULL,
            0x1eabdb9cd2b56268ULL, 0x488de20c93b1b8d5ULL, 0xb9f530fc75ac1c25ULL, 0xbcb8acaf5cf1bdd6ULL,
            0x04a8657f630fe9c2ULL, 0x0af48be792a8e213ULL, 0xcd5bed064a153125ULL, 0x2ddbfe990c44ae43ULL,
            0x0c4c037ca46afa32ULL, 0x451161416363f7f7ULL, 0x7a477b49540353b1ULL, 0x45f9f8b1d89a0b34ULL,
            0x7617691acd8cf3f6ULL, 0x63ccb55b4625d15cULL, 0xc0a785af64af504dULL, 0xa5ae5f01a8fba2c5ULL,
            0xca52694d597da73cULL, 0xbc2125226d4b9a3fULL, 0x9f646e90354d66c6ULL, 0x6286c83f6d549768ULL,
            0x946c049dc384a519ULL, 0xa8cc45dc604fcd07ULL, 0x40f1eae5718281c4ULL, 0x23e271d82ea3b9c0ULL,
            0x4caf9c27805943adULL, 0x055884e95caf2459ULL, 0x526cc8ee75d34ccfULL, 0xe904d6e2da9ae230ULL,
            0xea21e188a63e0fc8ULL, 0xac96526986f60d12ULL, 0x658361d2c84e13f9ULL, 0xa9e0685d680cc02bULL,
            0x314cc82e74f43db6ULL, 0x9d40e191cb8bdbebULL, 0x6a3ebb4416d7717eULL, 0xd2410151b8610d54ULL,
            0xa05075a69241450cULL, 0x2d050ab8d1df84d2ULL, 0x5cccb84c986c5bc9ULL, 0x28cdc80857844b7bULL,
            0xd8db7a55b80a0e1cULL, 0x395d03fc22f640feULL, 0xb8924ce538a4fcbfULL, 0xb230db70fefc9079ULL,
            0xdbc4542cad993f2dULL, 0x323a77611b9d7b7fULL, 0xeca6f36099242e65ULL, 0x1cb56faf343110c2ULL,
            0x4aefeb1a9459886eULL, 0x6901802ff957976aULL, 0x87d664abce89e4a5ULL, 0x6cc6b27952e55c24ULL,
            0xedfa565ee83c2564ULL, 0xf3acf7456192d593ULL, 0xff5ed2e8743f0fb5ULL, 0x773127ba724aafb5ULL,
            0xd9c78909b9a664ebULL, 0x1f9e0428946c8c3fULL, 0x88d27af90706644bULL, 0xe665fb4788378fa0ULL,
            0xe89d9daeddfcf314ULL, 0x37b3e7ceda0d4865ULL, 0x840129dea569b867ULL, 0x3e3210047e9fdd4aULL,
            0x39ae4c93f31a1046ULL, 0xf181a51b4bad49c7ULL, 0x297bcc473157c699ULL, 0x101de10d30f87e56ULL,
            0x8c155dcd88a72343ULL, 0x84c04db4f8dcc505ULL, 0x2d694606a0402a1fULL, 0xc3eb2f3b05616982ULL,
            0x08bb21fde167c6c0ULL, 0xea86e3dc8d5f65e8ULL, 0x1b8f5e477c75683eULL, 0x8130c78c9cf47163ULL,
            0xcde2f022688995b3ULL, 0x8ae56c1adf5b5f95ULL, 0x22ae33fdb275431aULL, 0xadffc291fc7b28c0ULL,
            0xd4156389f17f2d3aULL, 0x6691fbd1d1e71cd1ULL, 0xf33efe1cd81579feULL, 0x275b32c0209d3e3fULL,
            0xcc0ad72099a8021aULL, 0x6611cdbf4373b963ULL, 0x24c6e7a2bd469133ULL, 0x35312ed862dc29f2ULL,
            0xf1b558784be523e8ULL, 0x6883a8d725f380fdULL, 0xc6e27e7d9a8cbc28ULL, 0xba32fd62cc43cf51ULL,
            0xe4354ffb4b768d1bULL, 0xb17b08d62b4fa660ULL, 0x6b543f3364b70dc7ULL, 0x76df60c4779ad3b0ULL,
            0x5d00de19cc561dd2ULL, 0x4c0b212e4ae2c504ULL, 0xecf5d8bb8229096fULL, 0x170b38a0927fdc57ULL,
            0x116b1e3d4221164bULL, 0xb3cb8881cb367e07ULL, 0x73f8012bbecd4c80ULL, 0xfdf085129b364888ULL,
            0xd3ceb9d8c83c3198ULL, 0xb363704c6d96de96ULL, 0xf1b63d56c7265a42ULL, 0x12ed231c2a0d5891ULL,
            0x452443e8e2730782ULL, 0xb7965175e3c89d50ULL, 0xa84f912d6e21cea7ULL, 0x9fd1d71f4fa3b2b3ULL,
            0x8f1e4c4b9d89b8cdULL, 0xc307899914d0fcdbULL, 0x0ac7a2c9634deccfULL, 0x385c66404614d2f5ULL,
            0x7e72fc0e61e892e8ULL, 0x3234598c54b88b55ULL, 0xbce6ec11af3994a8ULL, 0x85bce28169b3a1b5ULL,
            0xba23a7f49b386628ULL, 0x95badf7b41ee1f18ULL, 0x94582fadb05328e0ULL, 0x3f829077e325f15cULL,
            0x5cc6511affa5d96eULL, 0x311e14f688723340ULL, 0x938f393c87d7f2f0ULL, 0x77c3e5f0c4475e9fULL,
            0xf3968bae9fbc2305ULL, 0xcb4a4348ea62f5a6ULL, 0xd59949f41b1fb69aULL, 0xc5d9fed65e2f58afULL,
            0xbc79a8c1729f9eb8ULL, 0x04db424860a4d86cULL, 0x55a10c798f9e8894ULL, 0x76fb6af80a248659ULL,
            0x53db58bbdb2a4e31ULL, 0x9b0de15919d922feULL, 0x521ccef25e31a7a0ULL, 0x28aeee57ad0e09e7ULL,
            0x49c1971a3e625784ULL, 0x22bcb61b3b08871aULL, 0x9e8c23353a48cde3ULL, 0x6ab85a7521a1a274ULL,
            0x50f47919e655fe26ULL, 0x765a5f7d264ff41fULL, 0x60f9bbf3fdf2d30cULL, 0xc565ea6e21e4fa42ULL,
            0x639444c07b384667ULL, 0x8241cb75666eb384ULL, 0x04d74756e367187dULL, 0x4cd9576f7d695d85ULL,
            0x2f809e8cef7a0a1dULL, 0xd551c313c30fdb10ULL, 0xa89fe70beee794f3ULL, 0x474fa27af19fb09bULL,
            0xb7c018c3d3bf9b3aULL, 0xcd63fd0f8ba29a17ULL, 0x60fbf62fd1e5038dULL, 0x02f04d25bfbc8a0dULL,
            0x6914de47c002fc58ULL, 0xa87c6c77795bf956ULL, 0x9c85fe1938b0f125ULL, 0xe38e3be2ba603429ULL,
            0x85c31f47936d0d33ULL, 0xff2bf2940bee36a7ULL, 0x6f6d3319f3fd98ffULL, 0x22d2bdaa5741b807ULL,
            0x294d37140f67ce6fULL, 0x0cdc0c068ed5f5b8ULL, 0xf9ff81f08781161eULL, 0x9b6d279713fb256fULL,
            0x64ea14df76b1bc29ULL, 0xc70b71ce223be406ULL, 0x564b03e3d16e0343ULL, 0x7a76ef5d892f3a28ULL,
            0xc2706726ff3c46adULL, 0x8cdbbdbfd55c0aceULL, 0xb9c0314638a658b0ULL, 0x8e2475c92ff7203eULL,
            0x78a7ceebad9cb039ULL, 0x5c198776f6a22f22ULL, 0x99283e2b2615bdcfULL, 0xc6c7bb40d41e2124ULL,
            0x3814709d378f53ccULL, 0x93205e8166967a1dULL, 0xa0b749939f59888cULL, 0xe3547ac1bfbc8463ULL,
            0xa427dcdcfe64d368ULL, 0x9cb93e96d6773b1fULL, 0xee6399c2df8e6fbcULL, 0xc56d61b98e356bb4ULL,
            0x3b3b4490afde1046ULL, 0x5c197b6ef5edfec0ULL, 0xce6f5551b9d0cd73ULL, 0x00a538672250ec07ULL,
            0x2eff72e1b136cd3bULL, 0x732cba2657acdc20ULL, 0xe30830ba3b2b3e04ULL, 0xebed849b1ef7efa4ULL
    };

    static constexpr uint64_t EnPassant[64] = {
            0x93bcedbff59d01ceULL, 0xc16f218df86c5a0dULL, 0xf7f9a2f399f958dcULL, 0xcd9af9fe983b8c22ULL,
            0xa38a45c098c1cbd4ULL, 0x9e73b912a0c99870ULL, 0xba394deadd2f59d0ULL, 0xe7ec0eab927d4f8bULL,
            0x78d5cd1e7ff5d17cULL, 0xf63a34ef514e9635ULL, 0xecc723658aeeb348ULL, 0xf00f97d7c5b32029ULL,
            0x9add949a8f7969e2ULL, 0x54c484115852c16dULL, 0x88deefc31c4bf69eULL, 0x46dda35d41ecfc59ULL,
            0x53c10ac0e33e87b4ULL, 0xc5ce3fbb36f0bb5dULL, 0x3d737acf9af96d26ULL, 0xf8caada14c0112f9ULL,
            0x64292d21bde71fa8ULL, 0x7e5fd4fb7f6d6010ULL, 0xf9c10fa14de1916cULL, 0x79f570c9a47de6bdULL,
            0x05b5fee3df7e2f65ULL, 0xfb4548f76916a553ULL, 0xf2ff701a7431a5a6ULL, 0x5befbc12a4ddab9dULL,
            0xac70335849d1f727ULL, 0x01b3d40023cf82d3ULL, 0x7d265e2c2a3b372aULL, 0x718d99a579eb5631ULL,
            0x56f9bd46a1f8568fULL, 0xb3eb3ef770a942ffULL, 0xa3e962bdf37981f0ULL, 0xf3c1df8b58eb3038ULL,
            0x30f1ba828fb8816dULL, 0xeab34f703bacf5d0ULL, 0x241941f3f28b2482ULL, 0x135074c8c868e77aULL,
            0xbe4b6ad71cc4cb56ULL, 0x6270df8c60ab78f9ULL, 0xe1ca8a84d293920bULL, 0x81c5275d72a0247eULL,
            0xd80e5ae699cfb0efULL, 0x176fafa5c68cc1dcULL, 0xbfa6d87cd4892857ULL, 0xf2157454a940e322ULL,
            0x315a9476dfc2fa51ULL, 0x7fd114713089ef04ULL, 0xc195de39fbdd59deULL, 0x75d01bdd881338a9ULL,
            0x5bf8a3442e5dc4bbULL, 0xc6dd85b38e7e9c88ULL, 0x3bd75c53e981d3a7ULL, 0x970087b8376212e7ULL,
            0xdaeb54fe45f70124ULL, 0x429adbeb0a06dd0fULL, 0xaa8d3163dfdd9c52ULL, 0xd902ee8e864de060ULL,
            0xd28088017c675c1bULL, 0x9569e87617b0fed5ULL, 0x71a6c8ce40c21837ULL, 0xcaf7bbae5e8522b3ULL
    };

    static constexpr uint64_t Castling[16] = {
            0x4eb4c0b8b9f1370fULL, 0xed41dea030316ddbULL, 0x8d673fd2d7d25e2cULL, 0x2a2c2d92c5c54855ULL,
            0x9ae78c7429c79e40ULL, 0xac9acb1c19d06767ULL, 0x4e05a226413bcad2ULL, 0x99e22324cc2c2ea8ULL,
            0x70a20d80b7d65403ULL, 0x1fde9b2366a7bb04ULL, 0x1454104d365c9bb7ULL, 0xa5e6579e2a8e1cedULL,
            0x751947af6e6fd04bULL, 0x0cde06b4b9247e9eULL, 0x5046c09718cd9c01ULL, 0x200d4a09ec323b21ULL
    };

    static constexpr uint64_t SideToMove = 0x897c24acf263ff62ULL;

public:
    static constexpr uint64_t piece(const Piece &piece, const uint8_t &index) {
        return Pieces[64 * piece.getValue() + index];
    }

    static constexpr uint64_t en_passant(const uint8_t &index) {
        return EnPassant[index];
    }

    static constexpr uint64_t castling(const uint8_t &castling_rights) {
        return Castling[castling_rights];
    }

    static constexpr uint64_t side_to_move() {
        return SideToMove;
    }

};

#endif
