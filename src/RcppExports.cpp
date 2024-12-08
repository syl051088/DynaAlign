// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// similarityMH
NumericMatrix similarityMH(CharacterVector sequences, int k, int n_hash);
RcppExport SEXP _DynaAlign_similarityMH(SEXP sequencesSEXP, SEXP kSEXP, SEXP n_hashSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type sequences(sequencesSEXP);
    Rcpp::traits::input_parameter< int >::type k(kSEXP);
    Rcpp::traits::input_parameter< int >::type n_hash(n_hashSEXP);
    rcpp_result_gen = Rcpp::wrap(similarityMH(sequences, k, n_hash));
    return rcpp_result_gen;
END_RCPP
}
// similarityNW
NumericMatrix similarityNW(CharacterVector sequences, std::string matrixName, int gapOpen, int gapExt);
RcppExport SEXP _DynaAlign_similarityNW(SEXP sequencesSEXP, SEXP matrixNameSEXP, SEXP gapOpenSEXP, SEXP gapExtSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type sequences(sequencesSEXP);
    Rcpp::traits::input_parameter< std::string >::type matrixName(matrixNameSEXP);
    Rcpp::traits::input_parameter< int >::type gapOpen(gapOpenSEXP);
    Rcpp::traits::input_parameter< int >::type gapExt(gapExtSEXP);
    rcpp_result_gen = Rcpp::wrap(similarityNW(sequences, matrixName, gapOpen, gapExt));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_DynaAlign_similarityMH", (DL_FUNC) &_DynaAlign_similarityMH, 3},
    {"_DynaAlign_similarityNW", (DL_FUNC) &_DynaAlign_similarityNW, 4},
    {NULL, NULL, 0}
};

RcppExport void R_init_DynaAlign(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
