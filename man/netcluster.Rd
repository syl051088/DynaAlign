% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/clusterbreak.R
\name{netcluster}
\alias{netcluster}
\title{Generate cluster id using graph network and louvain method}
\usage{
netcluster(
  pepmat,
  igraph_mode = "upper",
  igraph_weight = TRUE,
  louvain_resolution = 1.05,
  louvain_range_perc = 0,
  louvain_step = 0,
  louvain_itr = 3
)
}
\arguments{
\item{pepmat}{A square or upper triangular similarity matrix}

\item{igraph_mode}{Mode settings for igraph (default: "upper")}

\item{igraph_weight}{Weight setting (TRUE/NULL) for igraph (default: TRUE)}

\item{louvain_resolution}{Resolution setting for Louvain algorithm (default: 1.05)}

\item{louvain_range_perc}{Resolution sensitivity range (default: 0)}

\item{louvain_step}{Number of steps to break the sensitivity range (default: 0)}

\item{louvain_itr}{Number of iterations to run for each resolution (default: 3)}
}
\value{
A vector containing cluster assignment
}
\description{
Generate cluster id using graph network and louvain method
}
