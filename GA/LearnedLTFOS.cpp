//
//  LearnedLTFOS.cpp
//  GA
//
//  Created by Tom den Ottelander on 09/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "LearnedLTFOS.hpp"

using namespace std;
using namespace arma;


/* ------------------------ Learned Linkage Tree FOS ------------------------ */

LearnedLT_FOS::LearnedLT_FOS(){
    reinitializeOnNewRound = true;
}

vector<uvec> LearnedLT_FOS::getFOS(vector<Individual> &population){
    return GenerateLinkageTreeFOS(population[0].genotype.size(), population, NULL, false);
}

string LearnedLT_FOS::id() { return "LearnedLT"; }
string LearnedLT_FOS::toString(){ return "Learned Linkage Tree FOS"; }


// Original function implementation by Marco Virgolin (https://github.com/marcovirgolin/GP-GOMEA/blob/master/GOMEA/GOMEAFOS.cpp).
// Adapted by Tom den Ottelander.
vector<uvec> LearnedLT_FOS::GenerateLinkageTreeFOS(size_t problemLength, const std::vector<Individual>& population, arma::mat * MI_distribution_adjustments, bool fos_no_root_swap) {
    //Changed number_of_nodes to problemLength
    
    problemLength = population[0].genotype.size();

    vector<vector <size_t>> FOS;

    size_t pop_size = population.size();

    // phase 1: estimate similarity matrix
    vector<vector < double_t >> mi_matrix(problemLength, vector<double_t>(problemLength, 0.0));

    uword encode_number = 0;
    unordered_map<string, uword> values_to_int_map;
    values_to_int_map.reserve(1000000);

    // maximum number of constants to consider, the subsequent ones will be binned
    size_t max_constants = 100;
    unordered_set<double_t> constants;
    vector<double_t> constants_v;

    constants.reserve(max_constants);
    constants_v.reserve(max_constants);

    /* NOT NEEDED TO MY UNDERSTANDING.
    // pre-process the population: make a symbolic representation of the population nodes
    vector<vector < uword >> pop_nodes(pop_size);

    for (int i = 0; i < pop_size; i++) {

        vector<uword> nodes;
        vector<Node *> nnodes = population[i]->GetSubtreeNodes(false);

        for (size_t j = 0; j < problemLength; j++) {

            Node * n = nnodes[j];
            string v = n->GetValue();

            // use constant binning
            if (n->op->type == OperatorType::opTerminal && n->op->term_type == OperatorTerminalType::opTermConstant) {

                if (constants.size() < max_constants) {
                    constants.insert(((OpRegrConstant*) n->op)->GetConstant());

                    if (constants.size() >= max_constants && constants_v.empty()) {
                        constants_v.insert(constants_v.begin(), constants.begin(), constants.end());
                        sort(constants_v.begin(), constants_v.end());
                    }

                } else {
                    // find nearest constant, set it to that value
                    double_t q = ((OpRegrConstant*) n->op)->GetConstant();
                    auto lower = std::lower_bound(constants_v.begin(), constants_v.end(), q);

                    if (abs(*(lower - 1) - q) < abs(*lower - q)) {
                        v = to_string(*(lower - 1));
                    } else {
                        v = to_string(*lower);
                    }
                }
            }

            auto it = values_to_int_map.find(v);
            if (it == values_to_int_map.end()) {
                values_to_int_map[v] = encode_number;

                nodes.push_back(encode_number);

                encode_number++;
            } else {
                nodes.push_back(it->second);
            }
        }

        pop_nodes[i] = nodes;
    }
    */

    // build frequency table for symbol pairs
//    mat frequencies(values_to_int_map.size(), values_to_int_map.size(), fill::zeros);
    
    // 2 X 2 for binary problems I guess?
    mat frequencies(2, 2, fill::zeros);
    uword val_i, val_j;
    
    // 2 for binary problems?
    encode_number = 2;

    // measure frequencies of pairs & compute joint entropy
    for (uword i = 0; i < problemLength; i++) {
        for (uword j = i + 1; j < problemLength; j++) {
            for (uword p = 0; p < pop_size; p++) {
//                val_i = pop_nodes[p][i];
//                val_j = pop_nodes[p][j];
                val_i = population[p].genotype[i];
                val_j = population[p].genotype[j];

                frequencies.at(val_i, val_j) += 1.0;
            }

            double_t freq;
            for (uword k = 0; k < encode_number; k++) {
                for (uword l = 0; l < encode_number; l++) {
                    freq = frequencies(k, l);
                    if (freq > 0.0) {
                        freq = freq / pop_size;
                        mi_matrix[i][j] += -freq * log(freq);
                        frequencies.at(k, l) = 0.0; // reset the freq;
                    }
                }
            }
            mi_matrix[j][i] = mi_matrix[i][j];
        }

        for (uword p = 0; p < pop_size; p++) {
//            val_i = pop_nodes[p][i];
            val_i = population[p].genotype[i];
            frequencies.at(val_i, val_i) += 1.0;
        }

        double_t freq;
        for (uword k = 0; k < encode_number; k++) {
            for (uword l = 0; l < encode_number; l++) {
                freq = frequencies(k, l);
                if (freq > 0) {
                    freq = freq / pop_size;
                    mi_matrix[i][i] += -freq * log(freq);
                    frequencies.at(k, l) = 0.0; // reset the freq;
                }
            }
        }
    }

    // transform entropy into mutual information
    // apply correction if the pointer MI_distribution_adjustments is not null
    if (!MI_distribution_adjustments) {

        for (size_t i = 0; i < problemLength; i++) {
            for (size_t j = i + 1; j < problemLength; j++) {
                mi_matrix[i][j] = mi_matrix[i][i] + mi_matrix[j][j] - mi_matrix[i][j];
                mi_matrix[j][i] = mi_matrix[i][j];
            }
        }

    } else {

        if (MI_distribution_adjustments->n_elem == 0) {
            delete MI_distribution_adjustments->memptr();
            MI_distribution_adjustments->set_size(problemLength, problemLength);
            for (size_t i = 0; i < problemLength; i++) {
                (*MI_distribution_adjustments)(i, i) = 1.0 / mi_matrix[i][i];
                for (size_t j = i + 1; j < problemLength; j++) {
                    (*MI_distribution_adjustments)(i, j) = 2.0 / mi_matrix[i][j];
                }
            }
        }

        for (size_t i = 0; i < problemLength; i++) {
            mi_matrix[i][i] = mi_matrix[i][i] * (*MI_distribution_adjustments)(i, i);
            for (size_t j = i + 1; j < problemLength; j++) {
                mi_matrix[i][j] = mi_matrix[i][j] * (*MI_distribution_adjustments)(i, j);
            }
        }


        for (size_t i = 0; i < problemLength; i++) {
            for (size_t j = i + 1; j < problemLength; j++) {
                mi_matrix[i][j] = mi_matrix[i][i] + mi_matrix[j][j] - mi_matrix[i][j];
                mi_matrix[j][i] = mi_matrix[i][j];
            }
        }
    }

    // assemble the Linkage Tree with UPGMA
    FOS = BuildLinkageTreeFromSimilarityMatrix(problemLength, mi_matrix);

    FOS.pop_back(); // remove the root of the Linkage Tree (otherwise entire solution can be swapped during GOM)

    if (fos_no_root_swap) {
        for (size_t i = 0; i < FOS.size(); i++) {
            vector<size_t> * set = &FOS[i];
            auto it = find(set->begin(), set->end(), 0);
            if (it != set->end()) {
                set->erase(it);
            }
        }
        // remove empty sets and duplicates
        set<vector < size_t>> fos_set;
        for (vector<size_t> set : FOS) {
            if (!set.empty())
                fos_set.insert(set);
        }
        FOS = vector<vector < size_t >> (fos_set.begin(), fos_set.end());
    }

    bool no_univar = false;
    if (no_univar) {
        set<vector < size_t>> fos_set;
        for (vector<size_t> set : FOS) {
            if (set.size() > 1)
                fos_set.insert(set);
        }
        FOS = vector<vector < size_t >> (fos_set.begin(), fos_set.end());
    }

    /*cout << " >>> generated FOS:" << endl;
     for (vector<size_t> & F : FOS) {
     if (F.size() > 1) {
     cout << "     ";
     for (size_t idx : F) {
     cout << idx << " ";
     }
     cout << endl;
     }
     }*/

    return transformLinkageTreeFOS(FOS);
}

vector<vector<size_t>> LearnedLT_FOS::BuildLinkageTreeFromSimilarityMatrix(size_t number_of_nodes, vector<vector<double_t>> &sim_matrix) {
    // code taken from Peter's
    
    vector<vector<size_t> > FOS;
    
//    vector<size_t> random_order;
//    random_order.reserve(number_of_nodes);
//    for (size_t i = 0; i < number_of_nodes; i++)
//        random_order.push_back(i);
//
//    //TODO: Write this with my own random algorithm, don't use armadillo's rand.
//    random_shuffle(random_order.begin(), random_order.end());
    vector<int> random_order = Utility::getRandomlyPermutedArrayV2(number_of_nodes);
    
    vector<vector < int >> mpm(number_of_nodes, vector<int>(1));
    vector<int> mpm_number_of_indices(number_of_nodes);
    int mpm_length = number_of_nodes;
    
    for (size_t i = 0; i < number_of_nodes; i++) {
        mpm[i][0] = random_order[i];
        mpm_number_of_indices[i] = 1;
    }
    
    /* Initialize LT to the initial MPM */
    FOS.resize(number_of_nodes + number_of_nodes - 1);
    size_t FOSs_index = 0;
    for (int i = 0; i < mpm_length; i++) {
        FOS[FOSs_index] = vector<size_t>(mpm[i].begin(), mpm[i].end());
        FOSs_index++;
    }
    
    /* Initialize similarity matrix */
    vector<vector < double_t >> S_matrix(number_of_nodes, vector<double_t>(number_of_nodes));
    for (int i = 0; i < mpm_length; i++)
        for (int j = 0; j < mpm_length; j++)
            S_matrix[i][j] = sim_matrix[mpm[i][0]][mpm[j][0]];
    for (int i = 0; i < mpm_length; i++)
        S_matrix[i][i] = 0;
    
    vector<vector < int >> mpm_new;
    vector<int> NN_chain(number_of_nodes + 2, 0);
    int NN_chain_length = 0;
    short done = 0;
    
    while (!done) {
        if (NN_chain_length == 0) {
            NN_chain[NN_chain_length] = (int) Utility::getRand() * mpm_length;
            NN_chain_length++;
        }
        
        while (NN_chain_length < 3) {
            NN_chain[NN_chain_length] = DetermineNearestNeighbour(NN_chain[NN_chain_length - 1], S_matrix, mpm_number_of_indices, mpm_length);
            NN_chain_length++;
        }
        
        while (NN_chain[NN_chain_length - 3] != NN_chain[NN_chain_length - 1]) {
            NN_chain[NN_chain_length] = DetermineNearestNeighbour(NN_chain[NN_chain_length - 1], S_matrix, mpm_number_of_indices, mpm_length);
            if (((S_matrix[NN_chain[NN_chain_length - 1]][NN_chain[NN_chain_length]] == S_matrix[NN_chain[NN_chain_length - 1]][NN_chain[NN_chain_length - 2]])) && (NN_chain[NN_chain_length] != NN_chain[NN_chain_length - 2]))
                NN_chain[NN_chain_length] = NN_chain[NN_chain_length - 2];
            NN_chain_length++;
            if (NN_chain_length > number_of_nodes)
                break;
        }
        int r0 = NN_chain[NN_chain_length - 2];
        int r1 = NN_chain[NN_chain_length - 1];
        int rswap;
        if (r0 > r1) {
            rswap = r0;
            r0 = r1;
            r1 = rswap;
        }
        NN_chain_length -= 3;
        
        if (r1 < mpm_length) { /* This test is required for exceptional cases in which the nearest-neighbor ordering has changed within the chain while merging within that chain */
            vector<size_t> indices(mpm_number_of_indices[r0] + mpm_number_of_indices[r1]);
            //indices.resize((mpm_number_of_indices[r0] + mpm_number_of_indices[r1]));
            //indices.clear();
            
            int i = 0;
            for (int j = 0; j < mpm_number_of_indices[r0]; j++) {
                indices[i] = mpm[r0][j];
                i++;
            }
            for (int j = 0; j < mpm_number_of_indices[r1]; j++) {
                indices[i] = mpm[r1][j];
                i++;
            }
            
            FOS[FOSs_index] = indices;
            FOSs_index++;
            
            double_t mul0 = ((double_t) mpm_number_of_indices[r0]) / ((double_t) mpm_number_of_indices[r0] + mpm_number_of_indices[r1]);
            double_t mul1 = ((double_t) mpm_number_of_indices[r1]) / ((double_t) mpm_number_of_indices[r0] + mpm_number_of_indices[r1]);
            for (i = 0; i < mpm_length; i++) {
                if ((i != r0) && (i != r1)) {
                    S_matrix[i][r0] = mul0 * S_matrix[i][r0] + mul1 * S_matrix[i][r1];
                    S_matrix[r0][i] = S_matrix[i][r0];
                }
            }
            
            mpm_new = vector<vector < int >> (mpm_length - 1);
            vector<int> mpm_new_number_of_indices(mpm_length - 1);
            int mpm_new_length = mpm_length - 1;
            for (i = 0; i < mpm_new_length; i++) {
                mpm_new[i] = mpm[i];
                mpm_new_number_of_indices[i] = mpm_number_of_indices[i];
            }
            
            mpm_new[r0] = vector<int>(indices.begin(), indices.end());
            
            mpm_new_number_of_indices[r0] = mpm_number_of_indices[r0] + mpm_number_of_indices[r1];
            if (r1 < mpm_length - 1) {
                mpm_new[r1] = mpm[mpm_length - 1];
                mpm_new_number_of_indices[r1] = mpm_number_of_indices[mpm_length - 1];
                
                for (i = 0; i < r1; i++) {
                    S_matrix[i][r1] = S_matrix[i][mpm_length - 1];
                    S_matrix[r1][i] = S_matrix[i][r1];
                }
                
                for (int j = r1 + 1; j < mpm_new_length; j++) {
                    S_matrix[r1][j] = S_matrix[j][mpm_length - 1];
                    S_matrix[j][r1] = S_matrix[r1][j];
                }
            }
            
            for (i = 0; i < NN_chain_length; i++) {
                if (NN_chain[i] == mpm_length - 1) {
                    NN_chain[i] = r1;
                    break;
                }
            }
            
            mpm = mpm_new;
            mpm_number_of_indices = mpm_new_number_of_indices;
            mpm_length = mpm_new_length;
            
            if (mpm_length == 1)
                done = 1;
        }
    }
    
    return FOS;
}

int LearnedLT_FOS::DetermineNearestNeighbour(int index, vector<vector<double_t>> &S_matrix, vector<int> & mpm_number_of_indices, int mpm_length) {
    int i, result;
    
    result = 0;
    if (result == index)
        result++;
    for (i = 1; i < mpm_length; i++) {
        if (((S_matrix[index][i] > S_matrix[index][result]) || ((S_matrix[index][i] == S_matrix[index][result]) && (mpm_number_of_indices[i] < mpm_number_of_indices[result]))) && (i != index))
            result = i;
    }
    
    return ( result);
}


//TODO: Make sure that this method is not needed. Rewrite generateLTFOS such that it builds up a vector<uvec> FOS from the beginning.
vector<uvec> LearnedLT_FOS::transformLinkageTreeFOS(vector<vector<size_t>> FOS){
    vector<uvec> result;
    result.reserve(FOS.size());
    for (vector<size_t> vec : FOS){
        uvec newVec = uvec(vec.size());
        for (size_t i = 0; i < vec.size(); i++){
            newVec[i] = vec[i];
        }
        result.push_back(newVec);
    }
    return result;
}
