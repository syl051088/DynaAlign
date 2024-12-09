% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/clusterbreak.R
\name{clusterbreak}
\alias{clusterbreak}
\title{Generate clusters with specified sizes using graph network and louvain method}
\usage{
clusterbreak(
  pep,
  thresh = 0.8,
  k_size = 2,
  hash_size = 50,
  size_max = 10,
  size_min = 3,
  sens = 1.05,
  max_itr = 10000
)
}
\arguments{
\item{pep}{A vector of peptide sequences}

\item{thresh}{Threshold similarity score used to remove edge between two sequences if not similar enough}

\item{k_size}{k-mer size for MinHash algorithm (default: k_size = 2)}

\item{hash_size}{Hash function size for MinHash algorithm (default: hash_size = 50)}

\item{size_max}{Maximum size of cluster desired (default: size_max = 10)}

\item{size_min}{Minimum size of cluster desired (default: size_min = 3)}

\item{sens}{Resolution setting for Louvain algorithm (default: sens = 1.05)}

\item{max_itr}{Maximum function calls wanted before halting function execution (default: max_itr = 500)}
}
\value{
A nx2 matrix with a column containing n peptide sequences and their corresponding cluster assignment
}
\description{
Generate clusters with specified sizes using graph network and louvain method
}
