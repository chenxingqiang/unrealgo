
#ifndef SG_PROBCUT_H
#define SG_PROBCUT_H

#include "lib/Array.h"
#include "board/GoBlackWhite.h"
#include "board/GoPoint.h"
#include "SgABSearch.h"
#include "lib/SgStack.h"
#include "lib/SgVector.h"

class SgProbCut {
 public:
  static const int MAX_PROBCUT = 20;
  SgProbCut();
  struct Cutoff {
    float a, b, sigma;
    int shallow, deep;

    Cutoff() : shallow(-1), deep(-1) {}
  };
  void AddCutoff(const Cutoff &c);
  bool GetCutoff(int deep, int index, Cutoff &cutoff);
  float GetThreshold() const;
  bool IsEnabled() const;
  bool ProbCut(SgABSearch &search, int depth, int alpha, int beta,
               SgSearchStack &moveStack,
               bool *isExactValue, int *value);
  void SetEnabled(bool flag);
  void SetThreshold(float t);

 private:
  float m_threshold;
  bool m_enabled;
  GoArray<GoArray<Cutoff, MAX_PROBCUT + 1>, MAX_PROBCUT + 1> m_cutoffs;
  GoArray<int, MAX_PROBCUT + 1> m_cutoff_sizes;
};

inline SgProbCut::SgProbCut() {
  m_threshold = 1.0;
  m_enabled = false;
  for (int i = 0; i < MAX_PROBCUT + 1; ++i)
    m_cutoff_sizes[i] = 0;
}

inline void SgProbCut::AddCutoff(const Cutoff &c) {
  int i = m_cutoff_sizes[c.deep];
  m_cutoffs[c.deep][i] = c;
  ++m_cutoff_sizes[c.deep];
}

inline bool SgProbCut::GetCutoff(int deep, int index, Cutoff &cutoff) {
  if (deep > MAX_PROBCUT)
    return false;
  if (index >= m_cutoff_sizes[deep])
    return false;
  cutoff = m_cutoffs[deep][index];
  return true;
}

inline void SgProbCut::SetThreshold(float t) {
  m_threshold = t;
}

inline float SgProbCut::GetThreshold() const {
  return m_threshold;
}

inline void SgProbCut::SetEnabled(bool flag) {
  m_enabled = flag;
}

inline bool SgProbCut::IsEnabled() const {
  return m_enabled;
}

#endif // SG_PROBCUT_H
