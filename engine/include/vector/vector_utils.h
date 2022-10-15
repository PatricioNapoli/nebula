#pragma once

namespace nebula::vector_utils {
    template<typename T>
    void move(const vector<T>& from, vector<T>& to) {
        to.insert(to.end(), std::make_move_iterator(from.begin()), std::make_move_iterator(from.end()));
    }

    template<typename T>
    vector<T> merge(const vector<T>& one, const vector<T>& two) {
        vector<T> merge{};
        merge.insert(merge.end(), one.begin(), one.end());
        merge.insert(merge.end(), two.begin(), two.end());
        return merge;
    }
}
