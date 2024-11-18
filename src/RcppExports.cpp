// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// minhash_similarity_matrix
NumericMatrix minhash_similarity_matrix(CharacterVector sequences, int k, int num_hash);
RcppExport SEXP _DynaAlign_minhash_similarity_matrix(SEXP sequencesSEXP, SEXP kSEXP, SEXP num_hashSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type sequences(sequencesSEXP);
    Rcpp::traits::input_parameter< int >::type k(kSEXP);
    Rcpp::traits::input_parameter< int >::type num_hash(num_hashSEXP);
    rcpp_result_gen = Rcpp::wrap(minhash_similarity_matrix(sequences, k, num_hash));
    return rcpp_result_gen;
END_RCPP
}
// needleman_wunsch_score
double needleman_wunsch_score(const std :: string& sequence1, const std :: string& sequence2, int matchScore, int mismatchPenalty, int gapPenalty);
RcppExport SEXP _DynaAlign_needleman_wunsch_score(SEXP sequence1SEXP, SEXP sequence2SEXP, SEXP matchScoreSEXP, SEXP mismatchPenaltySEXP, SEXP gapPenaltySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std :: string& >::type sequence1(sequence1SEXP);
    Rcpp::traits::input_parameter< const std :: string& >::type sequence2(sequence2SEXP);
    Rcpp::traits::input_parameter< int >::type matchScore(matchScoreSEXP);
    Rcpp::traits::input_parameter< int >::type mismatchPenalty(mismatchPenaltySEXP);
    Rcpp::traits::input_parameter< int >::type gapPenalty(gapPenaltySEXP);
    rcpp_result_gen = Rcpp::wrap(needleman_wunsch_score(sequence1, sequence2, matchScore, mismatchPenalty, gapPenalty));
    return rcpp_result_gen;
END_RCPP
}
// calculateSimilarityMatrix
NumericMatrix calculateSimilarityMatrix(CharacterVector sequences);
RcppExport SEXP _DynaAlign_calculateSimilarityMatrix(SEXP sequencesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type sequences(sequencesSEXP);
    rcpp_result_gen = Rcpp::wrap(calculateSimilarityMatrix(sequences));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_DynaAlign_minhash_similarity_matrix", (DL_FUNC) &_DynaAlign_minhash_similarity_matrix, 3},
    {"_DynaAlign_needleman_wunsch_score", (DL_FUNC) &_DynaAlign_needleman_wunsch_score, 5},
    {"_DynaAlign_calculateSimilarityMatrix", (DL_FUNC) &_DynaAlign_calculateSimilarityMatrix, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_DynaAlign(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
