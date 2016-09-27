#ifndef snap_temporalmotifs_h
#define snap_temporalmotifs_h

#include "Snap.h"
#include "motifcluster.h"

class Counter2D {
 public:
  // Base constructor
  Counter2D(int m, int n) : m_(m), n_(n) {
    if (m * n > 0) {
      data_ = TIntV(m * n);
      data_.PutAll(0);
    }
  }
  Counter2D() : Counter2D(0, 0) {}
  // Copy assignment
  Counter2D& operator=(const Counter2D& that) {
    if (this != &that) {
      m_ = that.m();
      n_ = that.n();
      if (m_ * n_ > 0) {
	data_ = TIntV(m_ * n_);
	data_.PutAll(0);
	for (int j = 0; j < n_; ++j) {
	  for (int i = 0; i < m_; ++i) {
	    data_[i + j * m_] = that(i, j);
	  }
	}
      } else {
	data_ = TIntV();
      }
    }
    return *this;
  }

  int m() const { return m_; }
  int n() const { return n_; }
  
  const TInt& operator()(int i, int j) const { return data_[i + j * m_]; }
  TInt& operator()(int i, int j) { return data_[i + j * m_]; }

 private:
  int m_;
  int n_;
  TIntV data_;
};

class Counter3D {
 public:
  // Base constructor
  Counter3D(int m, int n, int p) : m_(m), n_(n), p_(p) {
    if (m * n * p > 0) {
      data_ = TIntV(m * n * p);
      data_.PutAll(0);
    }
  }
  Counter3D() : Counter3D(0, 0, 0) {}
  // Copy assignment
  Counter3D& operator=(const Counter3D& that) {
    if (this != &that) {
      m_ = that.m();
      n_ = that.n();
      p_ = that.p();
      if (m_ * n_ * p_ > 0) {
	data_ = TIntV(m_ * n_ * p_);
	for (int k = 0; k < p_; ++k) {      
	  for (int j = 0; j < n_; ++j) {
	    for (int i = 0; i < m_; ++i) {
	      data_[i + j * m_ + k * m_ * n_] = that(i, j, k);
	    }
	  }
	}
      } else {
	data_ = TIntV();
      }
    }
    return *this;
  }
  
  int m() const { return m_; }
  int n() const { return n_; }
  int p() const { return p_; }
  TIntV& data() { return data_; }
  
  const TInt& operator()(int i, int j, int k) const {
    return data_[i + j * m_ + k * m_ * n_];
  }
  TInt& operator()(int i, int j, int k) {
    return data_[i + j * m_ + k * m_ * n_];
  }

 private:
  int m_;
  int n_;
  int p_;
  TIntV data_;
};

class TemporalMotifCounter {
 public:
  TemporalMotifCounter() {}
  void LoadData(const TStr& filename);

  void ThreeEventEdgeCounts(double delta, TIntV& counts);
  void ThreeEventEdgeCounts(int u, int v, double delta,
			    TIntV& counts);

  void ThreeEventStarCounts(double delta, TIntV& counts);

 private:
  PNGraph static_graph_;
  // temporal_data_[i][j] --> list of timestamps on (i, j)
  // This data structure double stores to access (i, j) or (j, i)
  TVec< THash<TInt, TIntV> > temporal_data_;
};

class ThreeEventMotifCounter {
 public:
  ThreeEventMotifCounter(int size);
  void Count(const TIntV& event_string, const TIntV& timestamps,
             double delta);
  int FinalCount(int i, int j, int k) { return counts3_(i, j, k); }  

 private:
  void IncrementCounts(int event);
  void DecrementCounts(int event);
  TIntV counts1_;
  Counter2D counts2_;
  Counter3D counts3_;
  int size_;  // alphabet size
};

class ThreeEventStarMotifCounter {
 public:
  ThreeEventStarMotifCounter(int num_nodes);

  void Count(const TIntV& node, const TIntV& dir,
	     const TIntV& timestamps, double delta);
 private:
  void PopPre(int nbr, int dir);
  void PopPost(int nbr, int dir);
  void PushPre(int nbr, int dir);
  void PushPost(int nbr, int dir);
  void DecMiddleSum(int nbr, int dir);
  void IncMiddleSum(int nbr, int dir);
  void ProcessCurrent(int nbr, int dir);  


  Counter2D pre_sum_  = Counter2D(2, 2);
  Counter2D post_sum_ = Counter2D(2, 2);
  Counter2D mid_sum_  = Counter2D(2, 2);

  Counter3D pre_counts_  = Counter3D(2, 2, 2);
  Counter3D post_counts_ = Counter3D(2, 2, 2);
  Counter3D mid_counts_  = Counter3D(2, 2, 2);

  TVec< TIntV > pre_nodes_;
  TVec< TIntV > post_nodes_;  
};

#endif  // snap_temporalmotifs_h
