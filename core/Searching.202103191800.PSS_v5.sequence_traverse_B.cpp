//
// Created by Zhen Peng on 03/19/2020.
//

#include "Searching.202103191800.PSS_v5.sequence_traverse_B.h"
#define BREAKDOWN_PRINT

namespace PANNS {

/**
 * Input the data from the file.
 * @param filename
 */
void Searching::load_data_load(const char *filename)
{
    auto old_d = dimension_;
    DiskIO::load_data(
            filename,
            data_load_,
            num_v_,
            dimension_);
    if (old_d) {
        if (old_d != dimension_) {
            std::cerr << "Error: data dimension " << dimension_
                      << " is not equal to query dimension " << old_d << "." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * Input queries from the file.
 * @param filename
 */
void Searching::load_queries_load(const char *filename)
{
    auto old_d = dimension_;
    DiskIO::load_data(
            filename,
            queries_load_,
            num_queries_,
            dimension_);
    if (old_d) {
        if (old_d != dimension_) {
            std::cerr << "Error: query dimension " << dimension_
                      << " is not equal to data dimension " << old_d << "." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

///**
// * Input the NSG graph from the file, save as opt_graph_
// * Reference: https://github.com/ZJULearning/nsg/blob/master/src/index_nsg.cpp
// * @param filename
// */
//void Searching::load_nsg_graph(char *filename)
//{
//    std::ifstream fin(filename);
//    if (!fin.is_open()) {
//        std::cerr << "Error: cannot read file " << filename << " ." << std::endl;
//        exit(EXIT_FAILURE);
//    }
//    fin.read(reinterpret_cast<char *>(&width_), sizeof(unsigned));
//    fin.read(reinterpret_cast<char *>(&ep_), sizeof(unsigned));
//
//    data_bytes_ = (1 + dimension_) * sizeof(dataf);
//    neighbor_bytes_ = (1 + width_) * sizeof(idi);
//    vertex_bytes_ = data_bytes_ + neighbor_bytes_;
//    opt_nsg_graph_ = (char *) malloc(num_v_ * vertex_bytes_);
//    if (!opt_nsg_graph_) {
//        std::cerr << "Error: no enough memory for opt_nsg_graph_." << std::endl;
//        exit(EXIT_FAILURE);
//    }
//
//    idi v_id = 0;
//    num_e_ = 0;
//    char *base_location = opt_nsg_graph_;
//    while (true) {
//        idi degree;
//        fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
//        if (fin.eof()) {
//            break;
//        }
//        num_e_ += degree;
////        std::vector<idi> tmp_ngbrs(degree);
////        fin.read(reinterpret_cast<char *>(tmp_ngbrs.data()), degree * sizeof(unsigned));
//
//        // Norm and data
//        distf norm = compute_norm(data_load_ + v_id * dimension_);
////        distf norm = compute_norm(v_id);
//        std::memcpy(base_location, &norm, sizeof(distf)); // Norm
//        memcpy(base_location + sizeof(distf), data_load_ + v_id * dimension_, dimension_ * sizeof(dataf)); // Data
//        base_location += data_bytes_;
//
//        // Neighbors
//        memcpy(base_location, &degree, sizeof(idi)); // Number of neighbors
//        fin.read(base_location + sizeof(idi), degree * sizeof(unsigned)); // Neighbors
////        memcpy(location + sizeof(idi), tmp_ngbrs.data(), degree * sizeof(unsigned));
//        base_location += neighbor_bytes_;
//        ++v_id;
//    }
//    if (v_id != num_v_) {
//        std::cerr << "Error: NSG data has " << v_id
//                  << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
//        exit(EXIT_FAILURE);
//    }
//    free(data_load_);
//    data_load_ = nullptr;
////    ////////////////////////
////    idi v_id = 0;
////    num_e_ = 0;
////    while (true) {
////        idi degree;
////        fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
////        if (fin.eof()) {
////            break;
////        }
////        num_e_ += degree;
////
////        std::vector<idi> ngbrs(degree);
////        fin.read(reinterpret_cast<char *>(ngbrs.data()), degree * sizeof(unsigned));
//////        nsg_graph_.push_back(ngbrs);
//////        tmp_edge_list.push_back(ngbrs);
////        edge_list_.push_back(ngbrs);
////        ++v_id;
////    }
////    if (v_id != num_v_) {
////        std::cerr << "Error: NSG data has " << v_id
////                  << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
////        exit(EXIT_FAILURE);
////    }
//}

/*
 * Read NSG graph, save as index and neighbors.
 */
void Searching::load_common_nsg_graph(const char *filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: cannot read file " << filename << " ." << std::endl;
        exit(EXIT_FAILURE);
    }
    fin.read(reinterpret_cast<char *>(&width_), sizeof(unsigned));
    fin.read(reinterpret_cast<char *>(&ep_), sizeof(unsigned));

    std::vector< std::vector<idi> > edge_list(num_v_);
    // Read edge_list
    {
        common_nsg_vertex_base_ = (edgei *) malloc(num_v_ * sizeof(edgei));
        if (!common_nsg_vertex_base_) {
            fprintf(stderr, "Error: load_common_nsg_graph(): common_nsg_vertex_base_ malloc failed.\n");
            exit(EXIT_FAILURE);
        }
        edgei base_offset = 0;
        idi v_id = 0;
        num_e_ = 0;
        while (true) {
            idi degree;
            fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
            if (fin.eof()) {
                break;
            }
            common_nsg_vertex_base_[v_id] = base_offset;

            num_e_ += degree;
            base_offset += 1 + degree;

            edge_list[v_id].resize(degree);
            fin.read(reinterpret_cast<char *>(edge_list[v_id].data()), sizeof(unsigned) * degree);
            ++v_id;
        }
        if (v_id != num_v_) {
            std::cerr << "Error: for out degrees, NSG data has " << v_id
                      << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Copy neighbors
    {
        common_nsg_deg_ngbrs_ = (idi *) malloc((num_v_ + num_e_) * sizeof(idi));
        if (!common_nsg_deg_ngbrs_) {
            fprintf(stderr, "Error: load_common_nsg_graph(): common_nsg_deg_ngbrs_ malloc failed.\n");
            exit(EXIT_FAILURE);
        }
        idi *nsg_base = common_nsg_deg_ngbrs_;
        for (idi v_id = 0; v_id < num_v_; ++v_id) {
            idi degree = edge_list[v_id].size();
            *nsg_base++ = degree;
            memcpy(nsg_base, edge_list[v_id].data(), sizeof(unsigned) * degree);
            nsg_base += degree;
        }
//        fin.clear(std::ios_base::goodbit);
//        fin.seekg(2 * sizeof(unsigned), std::ios_base::beg);
//        idi v_id = 0;
//        while (fin.good()) {
//            idi degree;
//            fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
////            idi tmp_degree;
////            fin.read(reinterpret_cast<char *>(&tmp_degree), sizeof(unsigned));
////            fin.seekg(sizeof(unsigned), std::ios_base::cur); // skip the degree
//            if (!fin.good()) {
//                break;
//            }
////            idi degree = out_degrees[v_id++];
////            if (tmp_degree != degree) {
////                printf("tmp_degree: %u "
////                       "degree: %u\n",
////                       tmp_degree,
////                       degree);
////            }
//            ++v_id;
//            *nsg_base++ = degree;
//            fin.read(reinterpret_cast<char *>(nsg_base), degree * sizeof(unsigned));
//            nsg_base += degree;
//        }
//        if (v_id != num_v_) {
//            std::cerr << "Error: for neighbors, NSG data has " << v_id
//                      << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
//            exit(EXIT_FAILURE);
//        }
    }
}

void Searching::reorder_load_data()
{
    // Reorder data_load_
    dataf *data = (dataf *) malloc(static_cast<uint64_t>(num_v_) * dimension_ * sizeof(dataf));
    if (!data) {
        fprintf(stderr, "Error: cannot malloc %lu bytes.\n", static_cast<uint64_t>(num_v_) * dimension_ * sizeof(dataf));
        exit(EXIT_FAILURE);
    }

    for (idi old_i = 0; old_i < num_v_; ++old_i) {
        idi new_i = map_old_to_new_[old_i];
        dataf *new_base = data + (new_i * dimension_);
        dataf *old_base = data_load_ + (old_i * dimension_);
        memcpy(new_base, old_base, dimension_ * sizeof(dataf));
    }

    // Replace data_load_
    free(data_load_);
    data_load_ = data;
}

void Searching::load_true_NN_and_reorder(
        const char *filename,
        std::vector< std::vector< std::pair<idi, distf> > > &true_nn_list)
//        const std::vector< std::vector< std::pair<idi, distf> > > &old_true_nn_list,
//        std::vector< std::vector< std::pair<idi, distf> > > &new_true_nn_list)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    idi t_query_num;
    idi t_K;
//    unsigned t_K;
    fin.read(reinterpret_cast<char *>(&t_query_num), sizeof(t_query_num));
    fin.read(reinterpret_cast<char *>(&t_K), sizeof(t_K));
//    if (t_query_num != query_num) {
//        fprintf(stderr, "Error: query_num %u is not equal to the record %u in true-NN file %s\n",
//                query_num, t_query_num, filename);
//        exit(EXIT_FAILURE);
//    }
    if (t_query_num < num_queries_) {
        fprintf(stderr, "Error: t_query_num %u is smaller than num_queries_ %u\n", t_query_num, num_queries_);
        exit(EXIT_FAILURE);
    }
    if (t_K < 100) {
        fprintf(stderr, "Error: t_K %u is smaller than 100.\n", t_K);
        exit(EXIT_FAILURE);
    }

//    data = new unsigned[(size_t) t_query_num * (size_t) t_K];
    true_nn_list.resize(t_query_num);
    for (idi q_i = 0; q_i < t_query_num; ++q_i) {
        true_nn_list[q_i].resize(t_K);
    }

    for (unsigned q_i = 0; q_i < t_query_num; ++q_i) {
//        size_t offset = q_i * t_K;
        for (unsigned n_i = 0; n_i < t_K; ++n_i) {
            unsigned old_id;
            float dist;
            fin.read(reinterpret_cast<char *>(&old_id), sizeof(old_id));
            fin.read(reinterpret_cast<char *>(&dist), sizeof(dist));
            unsigned new_id = map_old_to_new_[old_id];
//            true_nn_list[q_i][n_i] = std::make_pair(id, dist);
            true_nn_list[q_i][n_i] = std::make_pair(new_id, dist);
        }
    }

    fin.close();

    //////////////////
//    idi num_queries = old_true_nn_list.size();
//    new_true_nn_list.resize(num_queries);
//    for (idi q_i = 0; q_i < num_queries; ++q_i) {
//        const auto &old_list = old_true_nn_list[q_i];
//        auto &new_list = new_true_nn_list[q_i];
//        idi size = old_list.size();
//        new_list.resize(size);
//        for (idi e_i = 0; e_i < size; ++e_i) {
//            idi old_e = old_list[e_i].first;
//            idi new_e = map_old_to_new_[old_e];
//            new_list[e_i] = std::make_pair(new_e, old_list[e_i].second);
////            new_list[e_i] = new_e;
//        }
//    }
}
//void Searching::reorder_true_NN(
//        const std::vector< std::vector<idi> > &old_true_nn_list,
//        std::vector< std::vector<idi> > &new_true_nn_list)
//{
//    idi num_queries = old_true_nn_list.size();
//    new_true_nn_list.resize(num_queries);
//    for (idi q_i = 0; q_i < num_queries; ++q_i) {
////        idi new_i = map_old_to_new_[old_i];
//        const auto &old_list = old_true_nn_list[q_i];
//        auto &new_list = new_true_nn_list[q_i];
//        idi size = old_list.size();
//        new_list.resize(size);
//        for (idi e_i = 0; e_i < size; ++e_i) {
//            idi old_e = old_list[e_i];
//            idi new_e = map_old_to_new_[old_e];
//            new_list[e_i] = new_e;
//        }
//    }
//}

void Searching::load_nsg_and_reorder(const char *filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: cannot read file " << filename << " ." << std::endl;
        exit(EXIT_FAILURE);
    }
    fin.read(reinterpret_cast<char *>(&width_), sizeof(unsigned));
    fin.read(reinterpret_cast<char *>(&ep_), sizeof(unsigned));

    // Get edge_list
    std::vector< std::vector<idi> > edge_list(num_v_);
    {
        // Read nsg file
        idi old_id = 0;
        while (true) {
            idi degree;
            fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
            if (fin.eof()) {
                break;
            }

            num_e_ += degree;
            idi new_id = map_old_to_new_[old_id];
            edge_list[new_id].resize(degree);
            fin.read((char *) edge_list[new_id].data(), sizeof(unsigned) * degree);
            for (idi e_i = 0; e_i < degree; ++e_i) {
                edge_list[new_id][e_i] = map_old_to_new_[edge_list[new_id][e_i]];
            }
            ++old_id;
//            {//test
//                if (0 == old_id % 1000000) {
//                    printf("==== edge_list: %.2f%%\n", 1.0 * old_id / num_v_ * 100.0);
//                }
//            }
        }
        {// test
            printf("==== edge_list: 100.00%%\n");
        }
        if (old_id != num_v_) {
            std::cerr << "Error: for out degrees, NSG data has " << old_id
                      << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

//    {
//        std::vector< std::vector<idi> > tmp_edge_list(num_v_);
//        // Read nsg file
//        idi v_id = 0;
//        while (true) {
//            idi degree;
//            fin.read(reinterpret_cast<char *>(&degree), sizeof(unsigned));
//            if (fin.eof()) {
//                break;
//            }
//
//            num_e_ += degree;
//
//            tmp_edge_list[v_id].resize(degree);
//            fin.read(reinterpret_cast<char *>(tmp_edge_list[v_id].data()), sizeof(unsigned) * degree);
//            ++v_id;
//        }
//        if (v_id != num_v_) {
//            std::cerr << "Error: for out degrees, NSG data has " << v_id
//                      << " vertices, but origin data has " << num_v_ << " vertices." << std::endl;
//            exit(EXIT_FAILURE);
//        }
//
//        // Reorder
//        for (idi old_i = 0; old_i < num_v_; ++old_i) {
//            idi new_i = map_old_to_new_[old_i];
//            idi out_degree = tmp_edge_list[old_i].size();
//            edge_list[new_i].resize(out_degree);
//            for (idi e_i = 0; e_i < out_degree; ++e_i) {
//                idi old_e = tmp_edge_list[old_i][e_i];
//                idi new_e = map_old_to_new_[old_e];
//                edge_list[new_i][e_i] = new_e;
//            }
//        }
//    }

    // Replace ep_ and width_

    {
//        ep_old_ = ep_;
        ep_ = map_old_to_new_[ep_];
        idi max_out_degree = 0;
        for (idi v_i = 0; v_i < threshold_opt_id_; ++v_i) {
            idi out_degree = edge_list[v_i].size();
            max_out_degree = std::max(max_out_degree, out_degree);
        }
        if (max_out_degree) {
            width_ = max_out_degree;
        }
        {//test
            printf("==== width: %u\n", width_);

        }
    }

    // Get opt_nsg_graph_
    edgei opt_num_e = 0;
    {
        data_bytes_ = (1 + dimension_) * sizeof(dataf);
        child_bytes_ = 1 * sizeof(idi) + (1 + dimension_) * sizeof(dataf);
        neighbor_bytes_ = 1 * sizeof(idi) + width_ * child_bytes_;
        vertex_bytes_ = data_bytes_ + neighbor_bytes_;
//        neighbor_bytes_ = 1 * sizeof(idi) + (1 + dimension_) * sizeof(dataf);
//        neighbor_bytes_ = (1 + width_) * sizeof(idi);
//        vertex_bytes_ = 1 * sizeof(idi) + width_ * neighbor_bytes_;
        uint64_t opt_bytes = static_cast<uint64_t>(threshold_opt_id_) * vertex_bytes_;
        opt_nsg_graph_ = (char *) malloc(opt_bytes);
        if (!opt_nsg_graph_) {
            fprintf(stderr,
                    "Error: no enough memory for opt_nsg_graph_ (%lu GB).\n",
                    (opt_bytes)>>30);
            exit(EXIT_FAILURE);
        }
        printf("==== opt_size(GB): %lu bytes: %lu\n", (opt_bytes)>>30, opt_bytes);

//        char *base_location = opt_nsg_graph_;
        for (idi v_i = 0; v_i < threshold_opt_id_; ++v_i) {
            char *base_location = opt_nsg_graph_ + v_i * vertex_bytes_;
            idi degree = edge_list[v_i].size();
            opt_num_e += degree;
            // Norm and data
            {
                dataf *v_data = data_load_ + v_i * dimension_;
                distf norm = compute_norm(v_data);
                memcpy(base_location, &norm, sizeof(distf)); // Norm
                memcpy(base_location + sizeof(distf), v_data, dimension_ * sizeof(dataf)); // Data
                base_location += data_bytes_;
            }
//
            // Out degree
            memcpy(base_location, &degree, sizeof(idi));
            base_location += sizeof(idi);
//
            // Child id, its norm, and its data
            for (idi e_i = 0; e_i < degree; ++e_i) {
                idi nb_id = edge_list[v_i][e_i];
                const float *child_data_ = data_load_ + nb_id * dimension_;
                distf norm = compute_norm(child_data_);
                memcpy(base_location, &nb_id, sizeof(idi)); // Child's id
                memcpy(base_location + sizeof(idi), &norm, sizeof(distf)); // Child's norm
                memcpy(base_location + sizeof(idi) + sizeof(distf), child_data_, dimension_ * sizeof(dataf)); // Child's data
                base_location += child_bytes_;
            }

//            // Neighbors
//            memcpy(base_location, &degree, sizeof(idi)); // Number of neighbors
//            memcpy(base_location + sizeof(idi), edge_list[v_i].data(), degree * sizeof(idi));
//            base_location += neighbor_bytes_;
        }
        {//test
            printf("==== opt_data: 100.00%%\n");
        }
    }

//    {
////        data_bytes_ = (1 + dimension_) * sizeof(dataf);
////        child_bytes_ = 1 * sizeof(idi) + (1 + dimension_) * sizeof(dataf);
//        neighbor_bytes_ = 1 * sizeof(idi) + (1 + dimension_) * sizeof(dataf);
////        neighbor_bytes_ = (1 + width_) * sizeof(idi);
//        vertex_bytes_ = 1 * sizeof(idi) + width_ * neighbor_bytes_;
//        opt_nsg_graph_ = (char *) malloc(threshold_opt_id_ * vertex_bytes_);
//        if (!opt_nsg_graph_) {
//            fprintf(stderr,
//                    "Error: no enough memory for opt_nsg_graph_ (%lu GB).\n",
//                    (threshold_opt_id_ * vertex_bytes_)>>30);
//            exit(EXIT_FAILURE);
//        }
//
////        char *base_location = opt_nsg_graph_;
//        for (idi v_i = 0; v_i < threshold_opt_id_; ++v_i) {
//            char *base_location = opt_nsg_graph_ + v_i * vertex_bytes_;
//            idi degree = edge_list[v_i].size();
//            opt_num_e += degree;
////            // Norm and data
////            distf norm = compute_norm(data_load_ + v_i * dimension_);
////            memcpy(base_location, &norm, sizeof(distf)); // Norm
////            memcpy(base_location + sizeof(distf), data_load_ + v_i * dimension_, dimension_ * sizeof(dataf)); // Data
////            base_location += data_bytes_;
////
//            // Out degree
//            memcpy(base_location, &degree, sizeof(idi));
//            base_location += sizeof(idi);
////
//            // Child id, its norm, and its data
//            for (idi e_i = 0; e_i < degree; ++e_i) {
//                idi nb_id = edge_list[v_i][e_i];
//                const float *child_data_ = data_load_ + nb_id * dimension_;
//                distf norm = compute_norm(child_data_);
//                memcpy(base_location, &nb_id, sizeof(idi)); // Child's id
//                memcpy(base_location + sizeof(idi), &norm, sizeof(distf)); // Child's norm
//                memcpy(base_location + sizeof(idi) + sizeof(distf), child_data_, dimension_ * sizeof(dataf)); // Child's data
//                base_location += neighbor_bytes_;
//            }
//
////            // Neighbors
////            memcpy(base_location, &degree, sizeof(idi)); // Number of neighbors
////            memcpy(base_location + sizeof(idi), edge_list[v_i].data(), degree * sizeof(idi));
////            base_location += neighbor_bytes_;
//        }
//    }
    {// test
        printf("==== common_nsg_ ... \n");
    }
    // Get common_nsg_, which starts from threshold_opt_id_ rather than 0.
    {
        idi remain_num_v = num_v_ - threshold_opt_id_;
        edgei remain_num_e = num_e_ - opt_num_e;
        // Get common_nsg_vertex_base_
        common_nsg_vertex_base_ = (edgei *) malloc(remain_num_v * sizeof(edgei));
        common_nsg_deg_ngbrs_ = (idi *) malloc((remain_num_e + remain_num_v) * sizeof(idi));
        if (!common_nsg_vertex_base_) {
            fprintf(stderr, "Error: load_and_reorder_nsg_graph(): common_nsg_vertex_base_ malloc failed.\n");
            exit(EXIT_FAILURE);
        }
        if (!common_nsg_deg_ngbrs_) {
            fprintf(stderr, "Error: load_and_reorder_nsg_graph(): common_nsg_deg_ngbrs_ malloc failed.\n");
            exit(EXIT_FAILURE);
        }
        edgei base_offset = 0;
        idi *nsg_base = common_nsg_deg_ngbrs_;
        for (idi v_i = 0; v_i < remain_num_v; ++v_i) {
            idi v_id = v_i + threshold_opt_id_;
            idi degree = edge_list[v_id].size();
            common_nsg_vertex_base_[v_i] = base_offset;
            base_offset += 1 + degree;

            *nsg_base++ = degree;
            memcpy(nsg_base, edge_list[v_id].data(), degree * sizeof(idi));
            nsg_base += degree;
//            {//test
//                if (0 == v_i % 1000000) {
//                    printf("==== common_nsg: %.2f%%\n", 1.0 * v_i / remain_num_v * 100.0);
//                }
//            }
        }
        {//test
            printf("==== common_nsg: 100.00%%\n");
        }
    }

//    // Cut the data_load_
//    {
//        uint64_t bytes = (num_v_ - threshold_opt_id_) * dimension_ * sizeof(dataf);
//        dataf *data = (dataf *) malloc(bytes);
//        if (!data) {
//            fprintf(stderr, "Error: load_and_reorder_nsg_graph(): cannot allocate data.\n");
//            exit(EXIT_FAILURE);
//        }
//        memcpy(data, data_load_ + threshold_opt_id_ * dimension_, bytes);
//
//        free(data_load_);
//        data_load_ = data;
//    }
}

void Searching::load_reorder_map(
        const char *filename)
{
//    //// for test
//    map_old_to_new_.resize(num_v_);
//    for (idi v_i = 0; v_i < num_v_; ++v_i) {
//        map_old_to_new_[v_i] = v_i;
//    }
//    //// End test

    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s .\n", filename);
        exit(EXIT_FAILURE);
    }
    fin.seekg(0, std::ios_base::end);
    uint64_t file_size = fin.tellg();
    idi num_v = file_size / (4 + 4);
//    {//test
//        printf("==== num_v: %u\n", num_v);
//    }
    map_old_to_new_.resize(num_v);

    // Set num_v_
    num_v_ = num_v;

////////////////////////////
//    map_old_to_new_.resize(num_v_);
//    std::ifstream fin(filename);
//    if (!fin.is_open()) {
//        fprintf(stderr, "Error: cannot open file %s .\n", filename);
//        exit(EXIT_FAILURE);
//    }
/////////////////////////////

    idi old_id;
    idi new_id;

    fin.seekg(0, std::ios_base::beg);
    for (idi v_i = 0; v_i < num_v; ++v_i) {
        fin.read(reinterpret_cast<char *>(&old_id), sizeof(old_id));
        fin.read(reinterpret_cast<char *>(&new_id), sizeof(new_id));
        map_old_to_new_[old_id] = new_id;
//        {// test
//            if (0 == v_i % 1000000) {
//                printf("==== old_id: %u new_id: %u\n", old_id, new_id);
//            }
//        }
    }
//    {// test
//        printf("==== %s done.\n", filename);
//    }
}

void Searching::load_data_and_reorder(
        const char *filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    uint64_t dim = 0;
//    idi dim;
    fin.read((char *) (&dim), 4);
    if (dimension_ && dim != dimension_) {
        fprintf(stderr, "Error: load_data_and_reorder(): dimension (%lu and %lu) is wrong.\n", dim, dimension_);
        exit(EXIT_FAILURE);
    }
    dimension_ = dim;
    fin.seekg(0, std::ios_base::end);
    uint64_t file_bytes = fin.tellg();
    idi num_v = file_bytes / (4 + 4 * dim);
    {//test
        printf("==== dim: %lu file_bytes: %lu num_v: %u\n", dim, file_bytes, num_v);
    }
    if (num_v_ && num_v != num_v_) {
        fprintf(
                stderr,
                "Error: load_data_and_reorder(): num_v (%u and %u) is wrong.\n",
                num_v, num_v_);
        exit(EXIT_FAILURE);
    }
    uint64_t data_bytes = static_cast<uint64_t>(num_v) * static_cast<uint64_t>(dim) * sizeof(dataf);
    dataf *data = (dataf *) malloc(data_bytes);
    if (!data) {
        fprintf(stderr, "Error: cannot malloc %lu bytes for data.\n", data_bytes);
        exit(EXIT_FAILURE);
    }

    fin.seekg(0, std::ios_base::beg);
    uint64_t vector_bytes = 4 * dim;
    dataf *buffer = (dataf *) malloc(vector_bytes);
    if (!buffer) {
        fprintf(stderr, "Error: cannot malloc %lu bytes for buffer.\n", 4 * dim);
        exit(EXIT_FAILURE);
    }
    for (uint64_t old_i = 0; old_i < num_v; ++old_i) {
        fin.seekg(4, std::ios_base::cur);
        fin.read((char *) (buffer), vector_bytes);
        uint64_t new_i = map_old_to_new_[old_i];
        memcpy(data + new_i * dim, buffer, vector_bytes);
//        {//test
//            if (0 == old_i % 1000000) {
//                printf("==== data_load_: %.2f%%\n", 1.0 * old_i / num_v * 100.0);
//            }
//        }
    }
    {//test
        printf("==== data_load_: 100.00%%\n");
    }

    data_load_ = data;
    free(buffer);
}

/**
 * Load those true top-K neighbors (ground truth) of queries
 * @param filename
 * @param[out] true_nn_list
 */
void Searching::load_true_NN(
        const char *filename,
        std::vector< std::vector< std::pair<idi, distf> > > &true_nn_list)
//        std::vector< std::vector<idi> > &true_nn_list)
//        unsigned &t_K)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    idi t_query_num;
    idi t_K;
//    unsigned t_K;
    fin.read(reinterpret_cast<char *>(&t_query_num), sizeof(t_query_num));
    fin.read(reinterpret_cast<char *>(&t_K), sizeof(t_K));
//    if (t_query_num != query_num) {
//        fprintf(stderr, "Error: query_num %u is not equal to the record %u in true-NN file %s\n",
//                query_num, t_query_num, filename);
//        exit(EXIT_FAILURE);
//    }
    if (t_query_num < num_queries_) {
        fprintf(stderr, "Error: t_query_num %u is smaller than num_queries_ %u\n", t_query_num, num_queries_);
        exit(EXIT_FAILURE);
    }
    if (t_K < 100) {
        fprintf(stderr, "Error: t_K %u is smaller than 100.\n", t_K);
        exit(EXIT_FAILURE);
    }

//    data = new unsigned[(size_t) t_query_num * (size_t) t_K];
    true_nn_list.resize(t_query_num);
    for (idi q_i = 0; q_i < t_query_num; ++q_i) {
        true_nn_list[q_i].resize(t_K);
    }

    for (unsigned q_i = 0; q_i < t_query_num; ++q_i) {
//        size_t offset = q_i * t_K;
        for (unsigned n_i = 0; n_i < t_K; ++n_i) {
            unsigned id;
            float dist;
            fin.read(reinterpret_cast<char *>(&id), sizeof(id));
            fin.read(reinterpret_cast<char *>(&dist), sizeof(dist));
            true_nn_list[q_i][n_i] = std::make_pair(id, dist);
//            true_nn_list[q_i][n_i] = id;
        }
    }

    fin.close();
}
//// backup
//void Searching::load_true_NN(
//        const char *filename,
//        std::vector< std::vector<idi> > &true_nn_list)
////        unsigned &t_K)
//{
//    std::ifstream fin(filename);
//    if (!fin.is_open()) {
//        fprintf(stderr, "Error: cannot open file %s\n", filename);
//        exit(EXIT_FAILURE);
//    }
//    idi t_query_num;
//    idi t_K;
////    unsigned t_K;
//    fin.read(reinterpret_cast<char *>(&t_query_num), sizeof(t_query_num));
//    fin.read(reinterpret_cast<char *>(&t_K), sizeof(t_K));
////    if (t_query_num != query_num) {
////        fprintf(stderr, "Error: query_num %u is not equal to the record %u in true-NN file %s\n",
////                query_num, t_query_num, filename);
////        exit(EXIT_FAILURE);
////    }
//    if (t_query_num < num_queries_) {
//        fprintf(stderr, "Error: t_query_num %u is smaller than num_queries_ %u\n", t_query_num, num_queries_);
//        exit(EXIT_FAILURE);
//    }
//    if (t_K < 100) {
//        fprintf(stderr, "Error: t_K %u is smaller than 100.\n", t_K);
//        exit(EXIT_FAILURE);
//    }
//
////    data = new unsigned[(size_t) t_query_num * (size_t) t_K];
//    true_nn_list.resize(t_query_num);
//    for (idi q_i = 0; q_i < t_query_num; ++q_i) {
//        true_nn_list[q_i].resize(t_K);
//    }
//
//    for (unsigned q_i = 0; q_i < t_query_num; ++q_i) {
////        size_t offset = q_i * t_K;
//        for (unsigned n_i = 0; n_i < t_K; ++n_i) {
//            unsigned id;
//            float dist;
//            fin.read(reinterpret_cast<char *>(&id), sizeof(id));
//            fin.read(reinterpret_cast<char *>(&dist), sizeof(dist));
////            data[offset + n_i] = id;
//            true_nn_list[q_i][n_i] = id;
//        }
//    }
//
//    fin.close();
//}

void Searching::get_recall_for_all_queries(
        const std::vector< std::vector< std::pair<idi, distf> > > &true_nn_list,
        const std::vector< std::vector< std::pair<idi, distf> > > &set_K_list,
        std::unordered_map<unsigned, double> &recalls,
        const idi L) const
{
    if (true_nn_list[0].size() < 100) {
        fprintf(stderr, "Error: Number of true nearest neighbors of a query is smaller than 100.\n");
        exit(EXIT_FAILURE);
    }
    recalls[1] = 0.0;
    recalls[5] = 0.0;
    recalls[10] = 0.0;
    recalls[20] = 0.0;
    recalls[50] = 0.0;
    recalls[100] = 0.0;

    const idi set_K_size = L < 100 ? L : 100;
    for (unsigned q_i = 0; q_i < num_queries_; ++q_i) {
        std::vector<bool> is_used(set_K_size, false);
        for (unsigned top_i = 0; top_i < 100; ++top_i) {
            idi true_id = true_nn_list[q_i][top_i].first;
            distf true_dist = true_nn_list[q_i][top_i].second;
            for (unsigned n_i = 0; n_i < set_K_size; ++n_i) {
                if (is_used[n_i]) {
                    continue;
                }
                idi found_id = set_K_list[q_i][n_i].first;
                distf found_dist = set_K_list[q_i][n_i].second;
//                if (found_id == true_id) {
                if (found_id == true_id || found_dist == true_dist) {
                    if (n_i < 1) recalls[1] += 1;
                    if (n_i < 5) recalls[5] += 1;
                    if (n_i < 10) recalls[10] += 1;
                    if (n_i < 20) recalls[20] += 1;
                    if (n_i < 50) recalls[50] += 1;
                    if (n_i < 100) recalls[100] += 1;
//                    break;
                    is_used[n_i] = true;
                }
            }
        }
    }
    recalls[1] /= 1.0 * num_queries_;
    recalls[5] /= 5.0 * num_queries_;
    recalls[10] /= 10.0 * num_queries_;
    recalls[20] /= 20.0 * num_queries_;
    recalls[50] /= 50.0 * num_queries_;
    recalls[100] /= 100.0 * num_queries_;
}
//void Searching::get_recall_for_all_queries(
//        const std::vector< std::vector<idi> > &true_nn_list,
//        const std::vector<std::vector<unsigned>> &set_K_list,
//        std::unordered_map<unsigned, double> &recalls,
//        const idi L) const
//{
//    if (true_nn_list[0].size() < 100) {
//        fprintf(stderr, "Error: Number of true nearest neighbors of a query is smaller than 100.\n");
//        exit(EXIT_FAILURE);
//    }
//    recalls[1] = 0.0;
//    recalls[5] = 0.0;
//    recalls[10] = 0.0;
//    recalls[20] = 0.0;
//    recalls[50] = 0.0;
//    recalls[100] = 0.0;
//
//    const idi set_K_size = L < 100 ? L : 100;
//    for (unsigned q_i = 0; q_i < num_queries_; ++q_i) {
//        for (unsigned top_i = 0; top_i < 100; ++top_i) {
//            unsigned true_id = true_nn_list[q_i][top_i];
//            for (unsigned n_i = 0; n_i < set_K_size; ++n_i) {
//                if (set_K_list[q_i][n_i] == true_id) {
//                    if (n_i < 1) recalls[1] += 1;
//                    if (n_i < 5) recalls[5] += 1;
//                    if (n_i < 10) recalls[10] += 1;
//                    if (n_i < 20) recalls[20] += 1;
//                    if (n_i < 50) recalls[50] += 1;
//                    if (n_i < 100) recalls[100] += 1;
//                }
//            }
//        }
//    }
//    recalls[1] /= 1.0 * num_queries_;
//    recalls[5] /= 5.0 * num_queries_;
//    recalls[10] /= 10.0 * num_queries_;
//    recalls[20] /= 20.0 * num_queries_;
//    recalls[50] /= 50.0 * num_queries_;
//    recalls[100] /= 100.0 * num_queries_;
//}


/**
 * Prepare init_ids and flags, as they are constant for all queries.
 * @param[out] init_ids
 * @param L
 */
void Searching::prepare_init_ids(
        std::vector<unsigned int> &init_ids,
        const unsigned L) const
{
//    idi num_ngbrs = get_out_degree(ep_);
//    edgei edge_start = nsg_graph_indices_[ep_];
//    // Store ep_'s neighbors as candidates
//    idi tmp_l = 0;
//    for (; tmp_l < L && tmp_l < num_ngbrs; tmp_l++) {
//        init_ids[tmp_l] = nsg_graph_out_edges_[edge_start + tmp_l];
//    }
//    std::unordered_set<idi> visited_ids;
    boost::dynamic_bitset<> is_selected(num_v_);
    idi init_ids_end = 0;

//    if (ep_ < threshold_opt_id_) {
//        // Hot vertices
//        const char *vertex_base = opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_;
//        idi out_degree = *((idi *) vertex_base);
//        vertex_base += sizeof(idi);
//
//        // Traverse neighbors
//        for (idi e_i = 0; e_i < out_degree && init_ids_end < L; ++e_i) {
//            const char *nb_base = vertex_base + e_i * child_bytes_;
//            idi nb_id = *((idi *) nb_base);
//            if (is_selected[nb_id]) {
//                continue;
//            }
//            is_selected[nb_id] = true;
//            init_ids[init_ids_end++] = nb_id;
//        }
//    } else {
//        // Cold vertices
////        const idi cand_id_common = ep_ - threshold_opt_id_;
//        const idi *out_edges = common_nsg_deg_ngbrs_ + common_nsg_vertex_base_[ep_ - threshold_opt_id_];
//        idi out_degree = *out_edges++;
//        // Traverse neighbors
//        for (idi e_i = 0; e_i < out_degree && init_ids_end < L; ++e_i) {
//            idi nb_id = out_edges[e_i];
//            if (is_selected[nb_id]) {
//                continue;
//            }
//            is_selected[nb_id] = true;
//            init_ids[init_ids_end++] = nb_id;
//        }
//    }

//    //////////////////////
////    idi *out_edges = common_nsg_deg_ngbrs_ + common_nsg_vertex_base_[ep_];
//////    idi *out_edges = (idi *) (opt_nsg_graph_ + ep_ * vertex_bytes_ + data_bytes_);
////    idi out_degree = *out_edges++;
//    idi out_degree = 0;
//    idi *out_edges = nullptr;
//    get_out_degree_and_edges(
//            ep_,
//            out_degree,
//            out_edges);
//    idi init_ids_end = 0;
////    for (; tmp_l < L && tmp_l < out_degree; tmp_l++) {
//    for (idi e_i = 0; e_i < out_degree && init_ids_end < L; ++e_i) {
////        idi v_id = out_edges[tmp_l];
//        idi v_id = out_edges[e_i];
//        if(is_selected[v_id]) {
//            continue;
//        }
//        is_selected[v_id] = true;
////        init_ids[tmp_l] = v_id;
//        init_ids[init_ids_end++] = v_id;
////        init_ids[tmp_l] = out_edges[tmp_l];
////        visited_ids.insert(init_ids[tmp_l]);
//    }
//    //////////////////////

//    for (idi i = 0; i < tmp_l; ++i) {
//        is_visited[init_ids[i]] = true;
//    }

    // If ep_'s neighbors are not enough, add other random vertices
//    idi tmp_id = ep_old_ + 1; // use tmp_id to replace rand().
    idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
    while (init_ids_end < L) {
//        tmp_id %= num_v_;
        if (tmp_id == num_v_) {
            tmp_id = 0;
        }
        idi v_id = tmp_id++;
//        v_id = map_old_to_new_[v_id];
        if (is_selected[v_id]) {
            continue;
        }
        is_selected[v_id] = true;
        init_ids[init_ids_end++] = v_id;
    }
//    idi tmp_id = ep_ + 1; // use tmp_id to replace rand().
//    while (init_ids_end < L) {
////        tmp_id %= num_v_;
//        if (tmp_id == num_v_) {
//            tmp_id = 0;
//        }
//        idi v_id = tmp_id++;
//        if (is_selected[v_id]) {
//            continue;
//        }
//        is_selected[v_id] = true;
//        init_ids[init_ids_end++] = v_id;
//    }
//    {//test
//        for (idi v_i = 0; v_i < init_ids_end; ++v_i) {
//            idi new_id = init_ids[v_i];
//            idi old_id = map_new_to_old_[new_id];
//            printf("%u: %u\n", v_i, old_id);
//        }
//        exit(EXIT_FAILURE);
//    }
}

// TODO: re-code in AVX-512
/*
 * Norm = a^2;
 * Ref: https://github.com/ZJULearning/nsg/blob/master/include/efanna2e/distance.h
 */
dataf Searching::compute_norm(
        const dataf *data) const
//        idi vertex_id)
//        const std::vector<PANNS::dataf> &data)
//        size_t loc_start,
//        idi dimension)
{
//    const dataf *a = data.data() + loc_start;
//    const dataf *a = data_load_ + vertex_id * dimension_;
//    idi size = dimension_;
    dataf result = 0;
//#define AVX_L2NORM(addr, dest, tmp) \
//    tmp = _mm256_load_ps(addr); \
//    tmp = _mm256_mul_ps(tmp, tmp); \
//    dest = _mm256_add_ps(dest, tmp);
#define AVX_L2NORM(addr, dest, tmp) \
    tmp = _mm256_loadu_ps(addr); \
    tmp = _mm256_mul_ps(tmp, tmp); \
    dest = _mm256_add_ps(dest, tmp);

    __m256 sum;
    __m256 l0, l1;
    unsigned D = (dimension_ + 7) & ~7U;
    unsigned DR = D % 16;
    unsigned DD = D - DR;
    const float *l = data;
    const float *e_l = l + DD;
    float unpack[8] __attribute__ ((aligned (32))) = {0, 0, 0, 0, 0, 0, 0, 0};

    sum = _mm256_load_ps(unpack);
//    sum = _mm256_loadu_ps(unpack);
    if (DR) { AVX_L2NORM(e_l, sum, l0); }
    for (unsigned i = 0; i < DD; i += 16, l += 16) {
        AVX_L2NORM(l, sum, l0);
        AVX_L2NORM(l + 8, sum, l1);
    }
    _mm256_store_ps(unpack, sum);
//    _mm256_storeu_ps(unpack, sum);
    result = unpack[0] + unpack[1] + unpack[2] + unpack[3] + unpack[4] + unpack[5] + unpack[6] + unpack[7];

    return result;
}

/*
 * Return = v_norm - 2*v*q
 * q^2 is omitted. v_norm has been computed by compute_norm().
 * Ref: https://github.com/ZJULearning/nsg/blob/master/include/efanna2e/distance.h
 */
dataf Searching::compute_distance_with_norm(
        const dataf *v_data,
        const dataf *q_data,
//        idi vertex_id,
//        idi query_id,
//        const std::vector<PANNS::dataf> &d_data,
//        const std::vector<PANNS::dataf> &q_data,
//        PANNS::idi d_start,
//        PANNS::idi q_start,
        const dataf vertex_norm) const
//        idi dimension)
{
//    idi size = dimension_;
    float result = 0;
//#define AVX_DOT(addr1, addr2, dest, tmp1, tmp2) \
//          tmp1 = _mm256_load_ps(addr1);\
//          tmp2 = _mm256_load_ps(addr2);\
//          tmp1 = _mm256_mul_ps(tmp1, tmp2); \
//          dest = _mm256_add_ps(dest, tmp1);
#define AVX_DOT(addr1, addr2, dest, tmp1, tmp2) \
          tmp1 = _mm256_loadu_ps(addr1);\
          tmp2 = _mm256_loadu_ps(addr2);\
          tmp1 = _mm256_mul_ps(tmp1, tmp2); \
          dest = _mm256_add_ps(dest, tmp1);

    __m256 sum;
    __m256 l0, l1;
    __m256 r0, r1;
    unsigned D = (dimension_ + 7) & ~7U;
    unsigned DR = D % 16;
    unsigned DD = D - DR;
    const float *l = v_data;
    const float *r = q_data;
//    const float *l = (float *) (opt_nsg_graph_ + vertex_id * vertex_bytes_ + sizeof(distf));
//    const float *r = queries_load_ + query_id * dimension_;
    const float *e_l = l + DD;
    const float *e_r = r + DD;
    float unpack[8] __attribute__ ((aligned (32))) = {0, 0, 0, 0, 0, 0, 0, 0};

    sum = _mm256_load_ps(unpack);
//    sum = _mm256_loadu_ps(unpack);
    if (DR) { AVX_DOT(e_l, e_r, sum, l0, r0); }

    for (unsigned i = 0; i < DD; i += 16, l += 16, r += 16) {
        AVX_DOT(l, r, sum, l0, r0);
        AVX_DOT(l + 8, r + 8, sum, l1, r1);
    }
    _mm256_store_ps(unpack, sum);
//    _mm256_storeu_ps(unpack, sum);
    result = unpack[0] + unpack[1] + unpack[2] + unpack[3] + unpack[4] + unpack[5] + unpack[6] + unpack[7];

    result = -2 * result + vertex_norm;

    return result;
}

/*
 * Return = (v - q)^2
 * Ref: https://github.com/ZJULearning/nsg/blob/master/include/efanna2e/distance.h
 */
dataf Searching::compute_distance(
        const dataf *v_data,
        const dataf *q_data) const
{
    float result = 0;
#define AVX_L2SQR(addr1, addr2, dest, tmp1, tmp2) \
      tmp1 = _mm256_loadu_ps(addr1);\
      tmp2 = _mm256_loadu_ps(addr2);\
      tmp1 = _mm256_sub_ps(tmp1, tmp2); \
      tmp1 = _mm256_mul_ps(tmp1, tmp1); \
      dest = _mm256_add_ps(dest, tmp1);

    __m256 sum;
    __m256 l0, l1;
    __m256 r0, r1;
    unsigned D = (dimension_ + 7) & ~7U;
    unsigned DR = D % 16;
    unsigned DD = D - DR;
    const float *l = v_data;
    const float *r = q_data;
    const float *e_l = l + DD;
    const float *e_r = r + DD;
    float unpack[8] __attribute__ ((aligned (32))) = {0, 0, 0, 0, 0, 0, 0, 0};

    sum = _mm256_load_ps(unpack);
    if(DR) { AVX_L2SQR(e_l, e_r, sum, l0, r0); }

    for (unsigned i = 0; i < DD; i += 16, l += 16, r += 16) {
        AVX_L2SQR(l, r, sum, l0, r0);
        AVX_L2SQR(l + 8, r + 8, sum, l1, r1);
    }
    _mm256_store_ps(unpack, sum);
    result = unpack[0] + unpack[1] + unpack[2] + unpack[3] + unpack[4] + unpack[5] + unpack[6] + unpack[7];

    return result;
}

//
// The difference from insert_into_queue is that add_into_queue will increase the queue size by 1.
// add_into_queue with a queue_start
idi Searching::add_into_queue(
        std::vector<PANNS::Candidate> &queue,
        const idi queue_start,
        idi &queue_size, // The insertion location starting from queue_start
        const idi queue_capacity, // The maximum capacity of queue, independent with queue_start.
        const PANNS::Candidate &cand)
{
    if (0 == queue_size) {
        queue[queue_start + queue_size++] = cand;
        return 0;
    }
    idi queue_end = queue_start + queue_size;
    // Find the insert location
    const auto it_loc = std::lower_bound(queue.begin() + queue_start, queue.begin() + queue_end, cand);
//    auto it_loc = std::lower_bound(queue.begin(), queue.begin() + queue_size, cand);
    idi insert_loc = it_loc - queue.begin();

    if (insert_loc != queue_end) {
        if (cand.id_ == it_loc->id_) {
            // Duplicate
            return queue_capacity;
        }
        if (queue_size >= queue_capacity) { // Queue is full
            --queue_size;
            --queue_end;
        }
    } else { // insert_loc == queue_end, insert at the end?
        if (queue_size < queue_capacity) { // Queue is not full
            // Insert at the end
            queue[insert_loc] = cand;
            ++queue_size;
            return queue_size - 1;
        } else { // Queue is full
            return queue_capacity;
        }
    }
    // Add into queue
    memmove(reinterpret_cast<char *>(queue.data() + insert_loc + 1),
            reinterpret_cast<char *>(queue.data() + insert_loc),
            (queue_end - insert_loc) * sizeof(Candidate));
    queue[insert_loc] = cand;
    ++queue_size;
    return insert_loc - queue_start;
}


void Searching::add_into_queue_at(
        const Candidate &cand,
        std::vector<Candidate> &queue,
        const idi insert_index, // The insertion location, independent with queue_start
        const idi queue_start,
        idi &queue_size, // The number of elements in queue, independent with queue_start
        const idi queue_length) // The maximum capacity of queue, independent with queue_start.
{
    const idi dest_index = queue_start + insert_index;
    if (queue_size == queue_length) {
        --queue_size;
    }
    memmove(reinterpret_cast<char *>(queue.data() + dest_index + 1),
            reinterpret_cast<char *>(queue.data() + dest_index),
            (queue_size - insert_index) * sizeof(Candidate));
    queue[dest_index] = cand;
    ++queue_size;
}

void Searching::insert_one_element_at(
        const Candidate &cand,
        std::vector<Candidate> &queue,
        const idi insert_index,
        const idi queue_start,
        const idi queue_size)
{
    const idi dest_index = queue_start + insert_index;
    memmove(reinterpret_cast<char *>(queue.data() + dest_index + 1),
            reinterpret_cast<char *>(queue.data() + dest_index),
            (queue_size - insert_index - 1) * sizeof(Candidate));
    queue[dest_index] = cand;
}


/* Function:
 * queue1_size is fixed.
 */
idi Searching::merge_two_queues_into_1st_queue_seq_fixed(
        std::vector<Candidate> &queue1,
        const idi queue1_start,
        const idi queue1_size,
        std::vector<Candidate> &queue2,
        const idi queue2_start,
        const idi queue2_size)
//        const idi limit_size)
{
    assert(queue1_size && queue2_size);
    // Record the lowest insert location.
    auto it_loc = std::lower_bound(
            queue1.begin() + queue1_start,
            queue1.begin() + queue1_start + queue1_size,
            queue2[queue2_start]);
    idi insert_index = it_loc - (queue1.begin() + queue1_start);
    if (insert_index == queue1_size) {
        return insert_index;
    } else if (insert_index == queue1_size - 1) {
        queue1[queue1_start + insert_index] = queue2[queue2_start];
        return insert_index;
    }

    // Insert the 1st of queue2
    if (queue2[queue2_start].id_ != it_loc->id_) {
        // Not Duplicate
        insert_one_element_at(
                queue2[queue2_start],
                queue1,
                insert_index,
                queue1_start,
                queue1_size);
    } else if (!queue2[queue2_start].is_checked_ && it_loc->is_checked_) {
        it_loc->is_checked_ = false;
    }
    if (queue2_size == 1) {
        return insert_index;
    }

    // Insert
    idi q_i_1 = insert_index + 1 + queue1_start;
    idi q_i_2 = queue2_start + 1;
    const idi q_i_1_bound = queue1_start + queue1_size;
    const idi q_i_2_bound = queue2_start + queue2_size;
//    const idi insert_i_bound = queue1_start + limit_size;
    for (idi insert_i = insert_index + 1; insert_i < queue1_size; ++insert_i) {
        if (q_i_1 >= q_i_1_bound || q_i_2 >= q_i_2_bound) {
            // queue1 or queue2 finished traverse. Rest o
            break;
        } else if (queue1[q_i_1] < queue2[q_i_2]) {
            ++q_i_1;
        } else if (queue2[q_i_2] < queue1[q_i_1]) {
            // Insert queue2[q_i_2] into queue1
            insert_one_element_at(
                    queue2[q_i_2++],
                    queue1,
                    insert_i,
                    queue1_start,
                    queue1_size);
            ++q_i_1;
        } else {
            // Duplicate
            if (!queue2[q_i_2].is_checked_ && queue1[q_i_1].is_checked_) {
                queue1[q_i_1].is_checked_ = false;
            }
            ++q_i_2;
            ++q_i_1;
        }
    }

    return insert_index;
}

/* Function:
 * queue1_size should be updated.
 * queue1_length should be provided.
 */
idi Searching::merge_two_queues_into_1st_queue_seq_incr(
        std::vector<Candidate> &queue1,
        const idi queue1_start,
        idi &queue1_size, // The number of element in queue1, independent with queue1_start.
        const idi queue1_length, // The maximum capacity of queue1, independent with queue1_start.
        std::vector<Candidate> &queue2,
        const idi queue2_start,
        const idi queue2_size)
//        const idi limit_size)
{
    assert(queue1_size && queue2_size);
    // Record the lowest insert location.
    auto it_loc = std::lower_bound(
            queue1.begin() + queue1_start,
            queue1.begin() + queue1_start + queue1_size,
            queue2[queue2_start]);
    idi insert_index = it_loc - (queue1.begin() + queue1_start);
    if (insert_index == queue1_size) {
        idi copy_count = (queue1_size + queue2_size > queue1_length) ?
                         queue1_length - queue1_size :
                         queue2_size;
        memmove(queue1.data() + queue1_start + queue1_size,
                queue2.data() + queue2_start,
                copy_count * sizeof(Candidate));
        queue1_size += copy_count;
        return insert_index;
    }
    if (queue2[queue2_start].id_ != it_loc->id_) {
        // Not Duplicate
        add_into_queue_at(
                queue2[queue2_start],
                queue1,
                insert_index,
                queue1_start,
                queue1_size,
                queue1_length);
    } else if (!queue2[queue2_start].is_checked_ && it_loc->is_checked_) {
        it_loc->is_checked_ = false;
    }
    if (queue2_size == 1) {
        return insert_index;
    }

    // Insert
    idi q_i_1 = insert_index + 1 + queue1_start;
    idi q_i_2 = queue2_start + 1;
    idi q_i_1_bound = queue1_start + queue1_size; // When queue1_size is updated, so should be q_i_1_bound.
    const idi q_i_2_bound = queue2_start + queue2_size;
//    idi insert_i;
    for (idi insert_i = insert_index + 1; insert_i < queue1_length; ++insert_i) {
        if (q_i_1 >= q_i_1_bound) {
            queue1_size += std::min(queue1_length - insert_i, q_i_2_bound - q_i_2);
            for ( ; insert_i < queue1_size; ++insert_i) {
                queue1[queue1_start + insert_i] = queue2[q_i_2++];
            }
            break;
        } else if (q_i_2 >= q_i_2_bound) {
            break;
        } else if (queue1[q_i_1] < queue2[q_i_2]) {
            ++q_i_1;
        } else if (queue2[q_i_2] < queue1[q_i_1]) {
            add_into_queue_at(
                    queue2[q_i_2++],
                    queue1,
                    insert_i,
                    queue1_start,
                    queue1_size,
                    queue1_length);
            ++q_i_1;
            q_i_1_bound = queue1_start + queue1_size;
        } else {
            // Duplicate
            if (!queue2[q_i_2].is_checked_ && queue1[q_i_1].is_checked_) {
                queue1[q_i_1].is_checked_ = false;
            }
            ++q_i_2;
            ++q_i_1;
        }
    }
    return insert_index;
}


/* Function:
 * Use large local_queues_array as a concatenation of all queues
 */
idi Searching::merge_all_queues_to_master(
        std::vector<Candidate> &set_L,
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes,
        const idi local_queue_capacity,
        const idi L) const
{
    idi nk = L;
    const idi master_start = local_queues_starts[num_threads_ - 1];
    idi &master_size = local_queues_sizes[num_threads_ - 1];
    const idi w_i_bound = num_threads_ - 1;
    for (idi w_i = 0; w_i < w_i_bound; ++w_i) {
        const idi w_start = local_queues_starts[w_i];
        idi &w_size = local_queues_sizes[w_i];
        if (0 == w_size) {
            continue;
        }
        idi r = merge_two_queues_into_1st_queue_seq_fixed(
                set_L,
                master_start,
                L,
                set_L,
                w_start,
                w_size);
        if (r < nk) {
            nk = r;
        }
        w_size = 0;
    }

    return nk;
}
//// Backup
//idi Searching::merge_all_queues_to_master(
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &local_queues_starts,
//        std::vector<idi> &local_queues_sizes,
//        const idi local_queue_capacity,
//        const idi L) const
//{
////    const int num_queues = num_threads_;
//    idi nk = L;
//    int size = 1 << (static_cast<idi>(log2(num_threads_)));
//    idi log2size = static_cast<idi>(log2(size));
//    for (idi d = 0; d < log2size; ++d) {
//        uint32_t by = 1 << (d + 1);
//#pragma omp parallel for
//        for (int i = 0; i < size; i += by) {
//            const idi ai = i + (1 << (d + 1)) - 1; // i + 2^(d+1) - 1
//            const idi a_start = local_queues_starts[ai];
//            idi &a_size = local_queues_sizes[ai];
////            idi a_start = ai * local_queue_length;
//            const idi bi = i + (1 << d) - 1; // i + 2^d - 1
//            const idi b_start = local_queues_starts[bi];
//            idi &b_size = local_queues_sizes[bi];
////            idi b_start = bi * local_queue_length;
//            if (0 == b_size) {
//                continue;
//            }
//            if (a_size == 0) {
//                std::copy(set_L.begin() + b_start,
//                          set_L.begin() + b_start + b_size,
//                          set_L.begin() + a_start); // Copy bi to ai
//                a_size = b_size;
//                b_size = 0;
//                continue;
//            }
//            if (ai != static_cast<idi>(num_threads_ - 1)) {
//                merge_two_queues_into_1st_queue_seq_incr(
//                        set_L,
//                        a_start,
//                        a_size,
//                        local_queue_capacity,
//                        set_L,
//                        b_start,
//                        b_size);
//            } else {
//                idi r = merge_two_queues_into_1st_queue_seq_fixed(
//                        set_L,
//                        a_start,
//                        L,
//                        set_L,
//                        b_start,
//                        b_size);
//                if (r < nk) {
//                    nk = r;
//                }
//            }
//            b_size = 0;
//        }
//    }
//    // Reset local_queues_sizes
//    // Not do this for Collector Idea or Selecting Idea
////    std::fill(local_queues_sizes.begin(), local_queues_sizes.end() - 1, 0);
////    std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);
//
//    return nk;
////    return r;
//}



/*
 * Function: pick master queue's top-M unchecked elements to top_m_candidates.
 * Used by interval merge.
 */
idi Searching::pick_top_m_to_workers(
//        const idi M,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes,
        const idi local_queue_capacity,
        const idi k_uc) const
{
    const idi last_queue_start = local_queues_starts[num_threads_ - 1];
    idi c_i_start = k_uc + last_queue_start;
    idi c_i_bound = last_queue_start + local_queues_sizes[num_threads_ - 1];
    idi top_m_count = 0;
    int dest_queue = 0;
    for (idi c_i = c_i_start; c_i < c_i_bound; ++c_i) {
        if (set_L[c_i].is_checked_) {
            continue;
        }
        ++top_m_count;
        if (num_threads_ - 1 != dest_queue) {
            set_L[local_queues_starts[dest_queue] + local_queues_sizes[dest_queue]++] = set_L[c_i];
            set_L[c_i].is_checked_ = true;
            if (local_queues_sizes[dest_queue] == local_queue_capacity) {
                break;
            }
            ++dest_queue;
        } else {
            dest_queue = 0;
        }
    }
    return top_m_count;
}

/*
 * Function: pick top-M unchecked candidates from queue
 */
void Searching::pick_top_m_unchecked(
        const idi M,
        const idi k_uc,
        std::vector<Candidate> &set_L,
        const idi local_queue_start,
        const idi local_queue_size,
        std::vector<idi> &top_m_candidates,
//        const idi top_m_candidates_start,
        idi &top_m_candidates_size,
        idi &last_k) const
{
    idi tmp_last_k = local_queue_size;
    idi tmc_size = 0;
    idi c_i_start = local_queue_start + k_uc;
    idi c_i_bound = local_queue_start + local_queue_size;
    // Pick top-M
    for (idi c_i = c_i_start; c_i < c_i_bound && tmc_size < M; ++c_i) {
        if (set_L[c_i].is_checked_) {
            continue;
        }
        tmp_last_k = c_i - local_queue_start; // Record the location of the last candidate selected.
        set_L[c_i].is_checked_ = true;
        top_m_candidates[tmc_size++] = set_L[c_i].id_;
    }
    last_k = tmp_last_k;
    top_m_candidates_size = tmc_size;
}

//void Searching::get_out_degree_and_edges(
//        const idi cand_id_global,
//        idi &out_degree,
//        idi *&out_edges) const
//{
//    if (cand_id_global < threshold_opt_id_) {
//        // Optimized index
//        out_edges = (idi *) (opt_nsg_graph_ + cand_id_global * vertex_bytes_ + data_bytes_);
//    } else {
//        // Common index
//        out_edges = common_nsg_deg_ngbrs_ + common_nsg_vertex_base_[cand_id_global - threshold_opt_id_];
//    }
//    out_degree = *out_edges++;
//}
//
//distf Searching::get_distance_to_query(
//        const idi v_id_global,
//        const dataf *query_data,
//        const dataf query_norm) const
//{
//    if (v_id_global < threshold_opt_id_) {
//        // Optimized index
//        dataf *nb_data = (dataf *) (opt_nsg_graph_ + v_id_global * vertex_bytes_);
//        dataf norm = *nb_data++;
//        return compute_distance_with_norm(nb_data, query_data, norm) + query_norm;
//    } else {
//        // Common index
//        dataf *nb_data = data_load_ + (v_id_global - threshold_opt_id_) * dimension_;
//        return compute_distance(nb_data, query_data);
//    }
//}

/*
 * Function: expand a candidate, visiting its neighbors.
 * Return the lowest adding location.
 */
idi Searching::expand_one_candidate(
        const int worker_id,
        const idi cand_id_global,
        const dataf *query_data,
        const dataf query_norm,
        const distf &dist_bound,
//        distf &dist_thresh,
        std::vector<Candidate> &set_L,
        const idi local_queue_start,
        idi &local_queue_size,
        const idi &local_queue_capacity,
        boost::dynamic_bitset<> &is_visited,
        uint64_t &local_count_computation)
{
    uint64_t tmp_count_computation = 0;
    idi nk = local_queue_capacity;
    if (cand_id_global < threshold_opt_id_) {
        // Hot vertices
        const char *vertex_base = opt_nsg_graph_ + cand_id_global * vertex_bytes_ + data_bytes_;
        idi out_degree = *((idi *) vertex_base);
        vertex_base += sizeof(idi);
        // Traverse neighbors
        for (idi e_i = 0; e_i < out_degree; ++e_i) {
            const char *nb_base = vertex_base + e_i * child_bytes_;
            idi nb_id = *((idi *) nb_base);
//            vertex_base += sizeof(idi);
            {// Sequential edition
                if (is_visited[nb_id]) {
                    continue;
                }
                is_visited[nb_id] = true;
            }
            nb_base += sizeof(idi);
            distf norm = *((distf *) nb_base);
            nb_base += sizeof(distf);
            ++tmp_count_computation;
            distf dist = compute_distance_with_norm((const dataf*) nb_base, query_data, norm) + query_norm;
            if (dist > dist_bound) {
//            if (dist > dist_bound || dist > dist_thresh) {
                continue;
            }
            Candidate cand(nb_id, dist, false);
            // Add to the local queue.
            idi r = add_into_queue(
                    set_L,
                    local_queue_start,
                    local_queue_size,
                    local_queue_capacity,
                    cand);
            if (r < nk) {
                nk = r;
            }
        }
    } else {
        // Cold vertices
        idi out_degree = width_;

//    for (idi e_i = 0; e_i < out_degree; ++e_i) {
//        idi nb_id = (cand_id_global + e_i) % num_v_;
//        _mm_prefetch((char *)(data_load_ + nb_id * dimension_), _MM_HINT_T0);
//    }

        for (idi e_i = 0; e_i < out_degree; ++e_i) {
            idi nb_id = (cand_id_global + e_i) % num_v_;
//        if (is_visited[nb_id]) {
//            continue;
//        }
//        is_visited[nb_id] = true;

            dataf *nb_data = data_load_ + nb_id * dimension_;
            ++tmp_count_computation;
            distf dist = compute_distance(nb_data, query_data);

            if (dist > dist_bound) {
//        if (dist > dist_bound || dist > dist_thresh) {
                continue;
            }
            Candidate cand(nb_id, dist, false);
            // Add to the local queue.
            idi r = add_into_queue(
                    set_L,
                    local_queue_start,
                    local_queue_size,
                    local_queue_capacity,
                    cand);
            if (r < nk) {
                nk = r;
            }
        }
//        const idi cand_id_common = cand_id_global - threshold_opt_id_;
//        const idi *out_edges = common_nsg_deg_ngbrs_ + common_nsg_vertex_base_[cand_id_global - threshold_opt_id_];
//        idi out_degree = *out_edges++;
//        // Traverse neighbors
//        for (idi e_i = 0; e_i < out_degree; ++e_i) {
//            idi nb_id = out_edges[e_i];
//            { // Sequential edition
//                if (is_visited[nb_id]) {
//                    continue;
//                }
//                is_visited[nb_id] = true;
//            }
//            dataf *nb_data = data_load_ + nb_id * dimension_;
//            ++tmp_count_computation;
//            distf dist = compute_distance(nb_data, query_data);
//            if (dist > dist_bound) {
////            if (dist > dist_bound || dist > dist_thresh) {
//                continue;
//            }
//            Candidate cand(nb_id, dist, false);
//            // Add to the local queue.
//            idi r = add_into_queue(
//                    set_L,
//                    local_queue_start,
//                    local_queue_size,
//                    local_queue_capacity,
//                    cand);
//            if (r < nk) {
//                nk = r;
//            }
//        }
    }
    local_count_computation += tmp_count_computation;

    return nk;

//    /////////////////////
//
//    idi out_degree = 0;
//    idi *out_edges = nullptr;
//    get_out_degree_and_edges(
//            cand_id_global,
//            out_degree,
//            out_edges);
//////    _mm_prefetch(opt_nsg_graph_ + cand_id_shifted * vertex_bytes_ + data_bytes_, _MM_HINT_T0);
////    idi *out_edges = common_nsg_deg_ngbrs_ + common_nsg_vertex_base_[cand_id_shifted];
//////    idi *out_edges = (idi *) (opt_nsg_graph_ + cand_id_shifted * vertex_bytes_ + data_bytes_);
////    idi out_degree = *out_edges++;
//////    for (idi n_i = 0; n_i < out_degree; ++n_i) {
//////        _mm_prefetch(opt_nsg_graph_ + out_edges[n_i] * vertex_bytes_, _MM_HINT_T0);
//////    }
//    idi nk = local_queue_capacity;
//
//    for (idi e_i = 0; e_i < out_degree; ++e_i) {
//        idi nb_id = out_edges[e_i];
//        { // Sequential edition
//            if (is_visited[nb_id]) {
//                continue;
//            }
//            is_visited[nb_id] = true;
//        }
//
//////        auto *nb_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + nb_id * vertex_bytes_);
//////        dataf norm = *nb_data++;
//////        distf dist = compute_distance_with_norm(nb_data, query_data, norm);
////        dataf *nb_data = data_load_ + nb_id * dimension_;
////        distf dist = compute_distance(nb_data, query_data);
//        ++tmp_count_computation;
//        distf dist = get_distance_to_query(
//                nb_id,
//                query_data,
//                query_norm);
//
////        if (dist > dist_bound) {
//        if (dist > dist_bound || dist > dist_thresh) {
//            continue;
//        }
//        Candidate cand(nb_id, dist, false);
//        // Add to the local queue.
//        idi r = add_into_queue(
//                set_L,
//                local_queue_start,
//                local_queue_size,
//                local_queue_capacity,
//                cand);
//        if (r < nk) {
//            nk = r;
//        }
//    }
//    local_count_computation += tmp_count_computation;
//
//    return nk;
}

void Searching::initialize_set_L_para(
        const dataf *query_data,
        const dataf query_norm,
        const idi L,
        std::vector<Candidate> &set_L,
        const idi set_L_start,
        idi &set_L_size,
        const std::vector<idi> &init_ids,
        boost::dynamic_bitset<> &is_visited)
{
    //#pragma omp parallel for
    for (idi c_i = 0; c_i < L; ++c_i) {
        is_visited[init_ids[c_i]] = true;
    }

//#pragma omp parallel for
//        for (idi v_i = 0; v_i < L; ++v_i) {
//            idi v_id = init_ids[v_i];
//            _mm_prefetch(opt_nsg_graph_ + v_id * vertex_bytes_, _MM_HINT_T0);
//        }

    // Get the distances of all candidates, store in the set set_L.
    uint64_t tmp_count_computation = 0;
#pragma omp parallel for reduction(+ : tmp_count_computation)
    for (unsigned i = 0; i < L; i++) {
        unsigned v_id = init_ids[i];

        if (v_id < threshold_opt_id_) {
            dataf *v_data = (dataf *) (opt_nsg_graph_ + v_id * vertex_bytes_);
            dataf norm = *v_data++;
            ++tmp_count_computation;
            distf dist = compute_distance_with_norm(v_data, query_data, norm) + query_norm;
            set_L[set_L_start + i] = Candidate(v_id, dist, false); // False means not checked.
        } else {
            dataf *v_data = data_load_ + v_id * dimension_;
            ++tmp_count_computation;
            distf dist = compute_distance(v_data, query_data);
            set_L[set_L_start + i] = Candidate(v_id, dist, false); // False means not checked.
        }
//        //////////////////////
////        auto *v_data = reinterpret_cast<dataf *>(opt_nsg_graph_ + v_id * vertex_bytes_);
////        dataf norm = *v_data++;
////        distf dist = compute_distance_with_norm(v_data, query_data, norm);
////        auto *v_data = data_load_ + v_id * dimension_;
////        distf dist = compute_distance(v_data, query_data);
//        dataf *v_data = data_load_ + v_id * dimension_;
//        ++tmp_count_computation;
//        distf dist = compute_distance(v_data, query_data);
////        distf dist = get_distance_to_query(
////                v_id,
////                query_data,
////                query_norm);
//        set_L[set_L_start + i] = Candidate(v_id, dist, false); // False means not checked.
    }
    count_distance_computation_ += tmp_count_computation;
//        threads_computations_[0] += tmp_count_computation;
//        tmp_count_computation = 0;
    std::sort(
            set_L.begin() + set_L_start,
            set_L.begin() + set_L_start + L);
    set_L_size = L;
}


///*
// * 8/31/2020-06:45
// * Simple Search by workders.
// * No thread limits, no M, M is just L.
// */
//void Searching::para_search_with_simple_v3_large_graph_scale_M(
////        const idi M,
////        const idi worker_M,
//        const idi query_id,
//        const idi K,
//        const idi L,
//        std::vector<Candidate> &set_L,
//        const std::vector<idi> &init_ids,
//        std::vector<idi> &set_K,
//        const idi local_queue_capacity, // Maximum size of local queue
//        const std::vector<idi> &local_queues_starts,
//        std::vector<idi> &local_queues_sizes, // Sizes of local queue
//        boost::dynamic_bitset<> &is_visited,
//        const idi subsearch_iterations,
//        std::vector<idi> &top_m_candidates)
//{
////    time_initialization_ -= WallTimer::get_time_mark();
//    const idi master_queue_start = local_queues_starts[num_threads_ - 1];
//    idi &master_queue_size = local_queues_sizes[num_threads_ - 1];
//    const dataf *query_data = queries_load_ + query_id * dimension_;
//
//    // Initialization Phase
////    printf("Initializing...\n");
//    initialize_set_L_para(
//            query_data,
//            L,
//            set_L,
//            master_queue_start,
//            master_queue_size,
//            init_ids,
//            is_visited);
////    time_initialization_ += WallTimer::get_time_mark();
//
////    idi top_m_candidates_end = 0;
//    const distf &last_dist = set_L[master_queue_start + master_queue_size - 1].distance_;
//    idi iter = 0; // for debug
//
////    printf("Searching...\n");
//    // Sequential Version
//    if (num_threads_ == 1) {
//        idi k = 0; // Index of first unchecked candidate.
//        idi r;
//        idi cand_id;
////        bool is_quota_done = false;
//        uint64_t tmp_count_computation = 0;
//        while (k < L) {
//            ++iter;
//            auto &cand = set_L[master_queue_start + k];
//            if (!cand.is_checked_) {
//                cand.is_checked_ = true;
//                cand_id = cand.id_;
//                r = expand_one_candidate(
//                        0,
//                        cand_id,
//                        query_data,
//                        last_dist,
//                        set_L,
//                        master_queue_start,
//                        master_queue_size,
//                        L,
//                        is_visited,
//                        tmp_count_computation);
////                        is_quota_done);
//                count_distance_computation_ += tmp_count_computation;
//                tmp_count_computation = 0;
//                ++count_hops_;
//            } else {
//                r = L;
//            }
//            if (r <= k) {
//                k = r;
//            } else {
//                ++k;
//            }
//        }
//    } else { // Parallel Version
////        // Divide computation cost from thread 0 to others
////        std::fill(
////                threads_computations_.begin(),
////                threads_computations_.end(),
////                threads_computations_[0] / num_threads_);
//
//        idi k_master = 0; // Index of first unchecked candidate.
//        idi para_iter = 0;
//        uint64_t tmp_count_computation = 0;
////        uint8_t count_workers_done = 0;
//
//        // Scale-M Phase
//        bool no_need_to_continue = false;
//        {
//            idi k = 0;
//            idi top_m_candidates_size = 0;
//            idi M = 1;
//            idi M_bound = num_threads_;
//
//            while (M < M_bound) {
//                ++iter;
//                idi last_k = L;
//
//                // Pick Top M
//                pick_top_m_unchecked(
//                        M,
//                        k,
//                        set_L,
//                        master_queue_start,
//                        master_queue_size,
//                        top_m_candidates,
//                        top_m_candidates_size,
//                        last_k);
//                if (!top_m_candidates_size) {
//                    no_need_to_continue = true;
//                    break;
//                }
//
//                // Expand
//                idi nk = L;
//#pragma omp parallel for reduction(+ : tmp_count_computation)
//                for (idi c_i = 0; c_i < top_m_candidates_size; ++c_i) {
//                    int w_i = omp_get_thread_num();
//                    const idi queue_capacity = num_threads_ - 1 != w_i ? local_queue_capacity : L;
//                    idi cand_id = top_m_candidates[c_i];
//                    idi r = expand_one_candidate(
//                            w_i,
//                            cand_id,
//                            query_data,
//                            last_dist,
//                            set_L,
//                            local_queues_starts[w_i],
//                            local_queues_sizes[w_i],
//                            queue_capacity,
//                            is_visited,
//                            tmp_count_computation);
//                    if (num_threads_ - 1 == w_i && r < nk) {
//                        nk = r;
//                    }
//                } // Expand
//                top_m_candidates_size = 0;
//                count_distance_computation_ += tmp_count_computation;
//                tmp_count_computation = 0;
//
//                // Merge
//                ++count_merge_;
//                idi r = merge_all_queues_to_master(
//                        set_L,
//                        local_queues_starts,
//                        local_queues_sizes,
//                        local_queue_capacity,
//                        L);
//                if (r < nk) {
//                    nk = r;
//                }
//                if (nk <= last_k) {
//                    k = nk;
//                } else {
//                    k = last_k + 1;
//                }
//
//                // Scale M
//                M *= 2;
//            }
//        }
//
//        while (!no_need_to_continue) {
//            ++iter;
//            ++para_iter;
////            {//test
////                printf("------- iter: %u -------\n", iter);
////            }
//            // Pick and copy top-M unchecked from Master to other workers
//            if (!pick_top_m_to_workers(
////                    M,
//                    set_L,
//                    local_queues_starts,
//                    local_queues_sizes,
//                    local_queue_capacity,
//                    k_master)) {
//                break;
//            }
//
////            count_workers_done = 0;
//            // Expand
//#pragma omp parallel reduction(+ : tmp_count_computation)
//            {
////                bool is_quota_done = false;
//                int w_i = omp_get_thread_num();
//                const idi local_queue_start = local_queues_starts[w_i];
//                idi &local_queue_size = local_queues_sizes[w_i];
//                const idi queue_capacity = num_threads_ - 1 != w_i ? local_queue_capacity : L;
//                idi k_uc = num_threads_ - 1 != w_i ? 0 : k_master;
//                idi cand_id;
//                idi r;
//                idi worker_iter = 0;
//                while (worker_iter < subsearch_iterations && k_uc < local_queue_size) {
//                    auto &cand = set_L[local_queue_start + k_uc];
//                    if (!cand.is_checked_) {
//                        cand.is_checked_ = true;
//                        ++worker_iter;
//                        cand_id = cand.id_;
//                        r = expand_one_candidate(
//                                w_i,
//                                cand_id,
//                                query_data,
//                                last_dist,
//                                set_L,
//                                local_queue_start,
//                                local_queue_size,
//                                queue_capacity,
//                                is_visited,
//                                tmp_count_computation);
////                                is_quota_done);
//                        if (r <= k_uc) {
//                            k_uc = r;
//                        } else {
//                            ++k_uc;
//                        }
//                    } else {
//                        ++k_uc;
//                    }
//                } // Expand Top-1
//                if (num_threads_ - 1 == w_i) {
//                    k_master = k_uc;
//                }
////                if (k_uc == local_queue_size || is_quota_done) {
////                    ++count_workers_done;
////                }
//            } // Workers
//            count_distance_computation_ += tmp_count_computation;
//            tmp_count_computation = 0;
//            // Merge
//            {
//                ++count_merge_;
//                idi r = merge_all_queues_to_master(
//                        set_L,
//                        local_queues_starts,
//                        local_queues_sizes,
//                        local_queue_capacity,
//                        L);
//                if (r <= k_master) {
//                    k_master = r;
//                }
//            }
//        } // Search Iterations
//    } // Parallel Phase
//
////    count_iterations_ += iter;
//
//#pragma omp parallel for
//    for (idi k_i = 0; k_i < K; ++k_i) {
//        set_K[k_i] = set_L[k_i + master_queue_start].id_;
//    }
//
//    {// Reset
////        std::fill(is_visited.begin(), is_visited.end(), 0);
//        is_visited.reset();
////        is_visited.clear_all();
////        std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);
////        std::fill(threads_computations_.begin(), threads_computations_.end(), 0);
//    }
////    {//test
////        printf("query_id: %u "
////               "iter: %u\n",
////               query_id,
////               iter);
////    }
////    {//test
////        if (1 == query_id) {
////            exit(1);
////        }
////    }
//}


void Searching::para_search_PSS_v5_large_graph_sequence_traverse_B(
//        const idi M,
//        const idi worker_M,
        const idi query_id,
        const idi K,
        const idi L,
        std::vector<Candidate> &set_L,
        const std::vector<idi> &init_ids,
        std::vector< std::pair<idi, distf> > &set_K,
//        std::vector<idi> &set_K,
        const idi local_queue_capacity, // Maximum size of local queue
        const std::vector<idi> &local_queues_starts,
        std::vector<idi> &local_queues_sizes, // Sizes of local queue
        boost::dynamic_bitset<> &is_visited,
        const idi subsearch_iterations,
        const uint64_t compt_thrd)
//        std::vector<idi> &top_m_candidates)
{
#ifdef BREAKDOWN_PRINT
    time_seq_ -= WallTimer::get_time_mark();
#endif
    uint64_t count_compt = 0;
    const idi master_queue_start = local_queues_starts[num_threads_ - 1];
    idi &master_queue_size = local_queues_sizes[num_threads_ - 1];
    const dataf *query_data = queries_load_ + query_id * dimension_;
    const dataf query_norm = compute_norm(query_data);

    // Initialization Phase
    initialize_set_L_para(
            query_data,
            query_norm,
            L,
//            local_queue_capacity,
            set_L,
            master_queue_start,
            master_queue_size,
            init_ids,
            is_visited);
    count_compt += L;
//    idi top_m_candidates_end = 0;
    const distf &last_dist = set_L[master_queue_start + master_queue_size - 1].distance_;
    idi iter = 0; // for debug

#ifdef BREAKDOWN_PRINT
    time_seq_ += WallTimer::get_time_mark();
#endif

    // Sequential Version
//    if (num_threads_ == 1) {
//        idi k = 0; // Index of first unchecked candidate.
//        idi r;
//        idi cand_id;
////        bool is_quota_done = false;
//        uint64_t tmp_count_computation = 0;
//        while (k < L) {
//            ++iter;
//            auto &cand = set_L[master_queue_start + k];
//            if (!cand.is_checked_) {
//                cand.is_checked_ = true;
//                cand_id = cand.id_;
//                r = expand_one_candidate(
//                        0,
//                        cand_id,
//                        query_data,
//                        last_dist,
//                        set_L,
//                        master_queue_start,
//                        master_queue_size,
//                        L,
//                        is_visited,
//                        tmp_count_computation);
////                        is_quota_done);
//                count_distance_computation_ += tmp_count_computation;
//                tmp_count_computation = 0;
//                ++count_hops_;
//            } else {
//                r = L;
//            }
//            if (r <= k) {
//                k = r;
//            } else {
//                ++k;
//            }
//        }
//    } else
    { // Parallel Version
//        // Divide computation cost from thread 0 to others
//        {//test
//            printf("q_i: %u\n", query_id);
//        }
#ifdef BREAKDOWN_PRINT
        time_seq_ -= WallTimer::get_time_mark();
#endif
        idi k_master = 0; // Index of first unchecked candidate.
        idi para_iter = 0;
        uint64_t tmp_count_computation = 0;
//        uint8_t count_workers_done = 0;

        // Sequential Start
        bool no_need_to_continue = false;
//        {
////            distf &last_dist = set_L[master_queue_start + master_queue_size - 1].distance_;
//            idi r;
////            idi seq_iter_bound = num_threads_;
//            const idi seq_iter_bound = 1;
//            while (iter < seq_iter_bound) {
//                ++iter;
//                if (k_master == L) {
//                    no_need_to_continue = true;
//                    break;
//                }
////                distf dist_thresh = last_dist;
//                auto &cand = set_L[master_queue_start + k_master];
//                if (!cand.is_checked_) {
//                    cand.is_checked_ = true;
//                    idi cand_id = cand.id_;
//                    r = expand_one_candidate(
//                            0,
////                            num_threads_ - 1,
//                            cand_id,
//                            query_data,
//                            query_norm,
//                            last_dist,
////                            dist_thresh,
//                            set_L,
//                            master_queue_start,
//                            master_queue_size,
//                            L,
//                            is_visited,
//                            tmp_count_computation);
////                        is_quota_done);
//                    count_distance_computation_ += tmp_count_computation;
//                    tmp_count_computation = 0;
////                    ++count_hops_;
//                } else {
//                    r = L;
//                }
//                if (r <= k_master) {
//                    k_master = r;
//                } else {
//                    ++k_master;
//                }
//            }
//        }
//        {//test
//            for (idi v_i = 0; v_i < master_queue_size; ++v_i) {
//                printf("id: %u dist: %f\n", map_new_to_old_[set_L[master_queue_start + v_i].id_], set_L[master_queue_start + v_i].distance_);
//            }
//        }
//        idi index_th = L - 1;
//        idi index_th = K / 3;
//        idi index_th = K * 2 / 3;
//        idi index_th= K;
//        idi index_th= K * 5 / 3 / num_threads_;
//        if (index_th >= local_queue_capacity) {
//            index_th = local_queue_capacity - 1;
//        }
//        if (index_th >= L) {
//            index_th = L - 1;
//        }
#ifdef BREAKDOWN_PRINT
        time_seq_ += WallTimer::get_time_mark();
#endif

        // Parallel Phase
        while (!no_need_to_continue) {
#ifdef BREAKDOWN_PRINT
            time_seq_ -= WallTimer::get_time_mark();
#endif
            ++iter;
            ++para_iter;
//            {//test
//                printf("------- iter: %u -------\n", iter);
//            }
            // Pick and copy top-M unchecked from Master to other workers
            if (!pick_top_m_to_workers(
//                    M,
                    set_L,
                    local_queues_starts,
                    local_queues_sizes,
                    local_queue_capacity,
                    k_master) || count_compt >= compt_thrd) {
#ifdef BREAKDOWN_PRINT
                time_seq_ += WallTimer::get_time_mark();
#endif
                break;
            }
//            distf dist_thresh = last_dist;
//            distf dist_thresh = set_L[master_queue_start + master_queue_size - 1].distance_;
#ifdef BREAKDOWN_PRINT
            time_seq_ += WallTimer::get_time_mark();
#endif

//            count_workers_done = 0;
            // Expand
#ifdef BREAKDOWN_PRINT
            time_expand_ -= WallTimer::get_time_mark();
#endif

#pragma omp parallel reduction(+ : tmp_count_computation)
            {
//                bool is_quota_done = false;
                int w_i = omp_get_thread_num();
                const idi local_queue_start = local_queues_starts[w_i];
                idi &local_queue_size = local_queues_sizes[w_i];
                const idi queue_capacity = num_threads_ - 1 != w_i ? local_queue_capacity : L;
                idi k_uc = num_threads_ - 1 != w_i ? 0 : k_master;
                idi cand_id;
                idi r;
                idi worker_iter = 0;
                while (worker_iter < subsearch_iterations && k_uc < local_queue_size) {
                    auto &cand = set_L[local_queue_start + k_uc];
                    if (!cand.is_checked_) {
                        cand.is_checked_ = true;
                        ++worker_iter;
                        cand_id = cand.id_;
                        r = expand_one_candidate(
                                w_i,
                                cand_id,
                                query_data,
                                query_norm,
                                last_dist,
//                                dist_thresh,
                                set_L,
                                local_queue_start,
                                local_queue_size,
                                queue_capacity,
                                is_visited,
                                tmp_count_computation);
                        if (r <= k_uc) {
                            k_uc = r;
                        } else {
                            ++k_uc;
                        }
                        cand.is_checked_ = false;
//                        if (
////                        if (w_i != num_threads_ - 1 &&
//                                local_queue_size > index_thresh_
//                                && set_L[local_queue_start + index_thresh_].distance_ < dist_thresh) {
//                            dist_thresh = set_L[local_queue_start + index_thresh_].distance_;
//                        }
////                        if (r >= index_thresh_) {
////                            break;
////                        }
//                        {//test
//                            printf("iter:%u cand_id:%u\n", iter, map_new_to_old_[cand_id]);
//                        }
                    } else {
                        ++k_uc;
                    }
                    if (tmp_count_computation >= compt_thrd) {
//                        printf("q_i: %u "
//                               "tmp_count_computation: %lu "
//                               "count_compt: %lu "
//                               "compt_thrd: %lu\n",
//                               query_id,
//                               tmp_count_computation,
//                               count_compt,
//                               compt_thrd);
//                        exit(0);
                        break;
                    }
                } // Expand Top-1
                if (num_threads_ - 1 == w_i) {
                    k_master = k_uc;
                }
//                if (k_uc == local_queue_size || is_quota_done) {
//                    ++count_workers_done;
//                }
            } // Workers
            count_compt += tmp_count_computation;
            count_distance_computation_ += tmp_count_computation;
            tmp_count_computation = 0;
#ifdef BREAKDOWN_PRINT
            time_expand_ += WallTimer::get_time_mark();
            time_merge_ -= WallTimer::get_time_mark();
#endif
            // Merge
            {
                ++count_merge_;
                idi r = merge_all_queues_to_master(
                        set_L,
                        local_queues_starts,
                        local_queues_sizes,
                        local_queue_capacity,
                        L);
                if (r <= k_master) {
                    k_master = r;
                }
            }
#ifdef BREAKDOWN_PRINT
            time_merge_ += WallTimer::get_time_mark();
#endif
        } // Search Iterations
    } // Parallel Phase

//    count_iterations_ += iter;
#ifdef BREAKDOWN_PRINT
    time_seq_ -= WallTimer::get_time_mark();
#endif
#pragma omp parallel for
    for (idi k_i = 0; k_i < K; ++k_i) {
        set_K[k_i].first = set_L[k_i + master_queue_start].id_;
        set_K[k_i].second = set_L[k_i + master_queue_start].distance_;
    }

    {// Reset
//        std::fill(is_visited.begin(), is_visited.end(), 0);
        is_visited.reset();
//        is_visited.clear_all();
//        std::fill(local_queues_sizes.begin(), local_queues_sizes.end(), 0);
//        std::fill(threads_computations_.begin(), threads_computations_.end(), 0);
    }
//    {//test
//        printf("query_id: %u "
//               "iter: %u\n",
//               query_id,
//               iter);
//        exit(0);
//    }
//    {//test
//        for (idi k_i = 0; k_i < K; ++k_i) {
//            printf("%u\n", set_K[k_i]);
//        }
//        if (1000 == query_id) {
//            exit(1);
//        }
//    }
#ifdef BREAKDOWN_PRINT
    time_seq_ += WallTimer::get_time_mark();
#endif
//    {//test
//        for (idi v_i = 0; v_i < master_queue_size; ++v_i) {
//            printf("id: %u dist: %f\n", map_new_to_old_[set_L[master_queue_start + v_i].id_], set_L[master_queue_start + v_i].distance_);
//        }
//    }
//    {//test
//        if (query_id == 0) {
//            exit(0);
//        }
//    }
}

//void Searching::test_reorder()
//{
//    std::vector<idi> map_new_to_old(num_v_);
//    for ()
//}

}// namespace PANNS