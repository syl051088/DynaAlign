#' Louvain Clustering function
#'
#' @param gin Input network.
#' @param res Louvain algorithm sensitivity.
#' @param res_range_perc Percent of sensitivity for the function loop over for best modularity.
#' @param res_step Number of even breaks over the sensitivity range.
#' @param itr Number of iteration over each break point. 
#' 
#' @return A clustered graph network optimized for modularity
#' @return List containing:
#'   \item{cluster}{Final sequence cluster assignment}
#'   \item{resolution}{Best Louvain algorithm resolution found}
#'   \item{modularity}{Modularity using the best resolution}
#' @export
#' 
#' @importFrom igraph graph_from_adjacency_matrix E cluster_louvain V layout_with_fr
#' 
#' @examples
#' # Load necessary libraries
#' library(DynaAlign)
#' library(igraph)
#'
#' # Create a mock adjacency matrix
#' # This example creates a simple network with two clusters
#' adjacency_matrix <- matrix(c(
#'   1, 1, 0, 0,
#'   1, 1, 0, 0,
#'   0, 0, 1, 1,
#'   0, 0, 1, 1
#' ), nrow = 4, byrow = TRUE)
#'
#' # Create an igraph object from the adjacency matrix
#' network <- graph_from_adjacency_matrix(adjacency_matrix, mode = "undirected", weighted = TRUE)
#'
#' # Perform Louvain clustering with mock resolution parameters
#' result <- louvain_mod(network, res = 1.0, res_range_perc = 0.2, res_step = 0.1, itr = 2)
louvain_mod <- function(gin, res, res_range_perc = 0, res_step = 0, itr = 3) {
  
  res=seq(res - res_range_perc*res, res + res_range_perc*res, by = res_step)# set range of resolution parameters
  
  best_modularity <- NULL
  best_clusters <- NULL
  best_resolution<-NULL
  for (j in seq_along(res)){
    if (j==1){
      best_resolution<-res[j]
    }
    for (i in 1:itr) {#iteration loop
      gin.cluster<-igraph::cluster_louvain(gin,weights=igraph::E(gin)$weight,resolution=res[j]) # cluster using weights
      gin.mod<-  igraph::modularity(gin.cluster)#obtain modularity
      
      if(i==1){#initial loop
        best_modularity <- gin.mod
        best_clusters <- gin.cluster
      }
      if (i>1 & gin.mod > best_modularity) {#replace initial values if higher modularity
        best_modularity <- gin.mod
        best_clusters <- gin.cluster
        best_resolution <- res[j]
      }
    }
  }
  
  return(list(cluster=best_clusters,
              resolution=best_resolution,
              modularity=best_modularity))
}

#' Generate cluster ID using graph network and Louvain method
#'
#' @param pepmat A square or upper triangular adjacency matrix.
#' @param igraph_mode Mode settings for igraph (default: "upper").
#' @param igraph_weight Weight setting (TRUE/NULL) for igraph (default: TRUE).
#' @param cluster_func Function to perform network-based clustering compatible with igraph networks.
#' @param cluster_weight Logical value indicating whether to use network edge weights in the clustering function.
#'
#' @return A numeric vector containing cluster assignments.
#' @export
#'
#' @importFrom igraph graph_from_adjacency_matrix E cluster_louvain
#' 
#' @examples
#' # Load necessary libraries
#' library(DynaAlign)
#' library(igraph)
#'
#' # Create a mock adjacency matrix (similarity matrix)
#' # This example creates a simple network with two distinct clusters
#' adjacency_matrix <- matrix(c(
#'   1, 1, 0, 0,
#'   1, 1, 0, 0,
#'   0, 0, 1, 1,
#'   0, 0, 1, 1
#' ), nrow = 4, byrow = TRUE)
#'
#' # Perform clustering using the default Louvain method
#' default_clusters <- netcluster(pepmat = adjacency_matrix)
#' print(default_clusters)
#'
#' # Define a mock Louvain clustering function (replace with actual implementation)
#' mock_louvain_mod <- function(gin, res, ...) {
#'   # For demonstration, assign first two nodes to cluster 1 and the rest to cluster 2
#'   return(c(1, 1, 2, 2))
#' }
#'
#' # Perform clustering using the custom mock Louvain method without using edge weights
#' custom_clusters <- netcluster(
#'   pepmat = adjacency_matrix,
#'   cluster_weight = FALSE,
#'   cluster_func = function(x, ...) mock_louvain_mod(gin = x, res = 1.05, ...)
#' )
netcluster<-function(pepmat,
                     igraph_mode = "upper",
                     igraph_weight = TRUE,
                     cluster_func = function(x,...) igraph::cluster_louvain(x,
                                                                            resolution=1.05,...)$membership,
                     cluster_weight = TRUE) {
  if(nrow(pepmat)!=ncol(pepmat)) {
    stop("Input must be a square pairwise similarity matrix")
  }
  
  network<-igraph::graph_from_adjacency_matrix(pepmat,
                                               mode=igraph_mode,
                                               weighted=igraph_weight) #construct network
  if (cluster_weight){
    out <- cluster_func(network,weights=igraph::E(network)$weight) #include network weight in cluster function
  }else{
    out <- cluster_func(network)
  }
  
  if (is.numeric(out) && is.vector(out)){
    return(out)
  }else{
    stop("Wrong clustering output format. Output should be a numeric vector of cluster assignment.")
  }
}

#' Generate clusters with specified sizes using graph network and louvain method
#' 
#' @param pep A vector of amino acid sequences
#' @param thresh_p Quadrille of similarity score to set as threshold for adjacency 
#' @param size_max Maximum size of cluster desired (default: size_max = 10)
#' @param size_min Minimum size of cluster desired (default: size_min = 3)
#' @param max_itr Maximum function calls wanted before halting function execution (default: max_itr = 500)
#' @param sim_fn Function for generating similarity matrix (default: similarityMH)
#' @param cluster_fn Function for network-based clustering compatible with igraph object that must output a numeric vector of cluster assignment (default: cluster_louvain)
#' @param cluster_wt Logical value for whether or not the cluster function takes in network weights in the function
#' 
#' @return List containing:
#'   \item{clustered_seq}{A nx2 matrix containging selected sequences with their cluster assignments}
#'   \item{filtered_seq}{Filtered sequences}
#' @export
#' 
#' @importFrom igraph graph_from_adjacency_matrix E cluster_louvain
#' @importFrom stats quantile
#' 
#' @examples
#' library(DynaAlign)
#' library(dplyr) # Ensure dplyr is loaded for the pipe operator
#' 
#' # Create mock dataset
#' h3n2sample <- data.frame(
#'   clade = sample(c("A", "B", "C"), 1000, replace = TRUE),
#'   sequence = replicate(1000, paste(sample(LETTERS, 10, replace = TRUE), collapse = ""))
#' )
#' 
#' # Select and prepare sequences
#' test <- h3n2sample %>%
#'   group_by(clade) %>%
#'   sample_frac(0.4) %>%
#'   distinct(sequence, .keep_all = TRUE)
#'
#' # Cluster sequences
#' clusterbreak(
#'   pep = test$sequence,
#'   size_max = 800,
#'   thresh_p = 0.8,
#'   sim_fn = function(x) similarityMH(x, k = 4, n_hash = 500)
#' )
clusterbreak <- function(pep, 
                         thresh_p = 0.8,
                         size_max = 10, 
                         size_min = 3, 
                         max_itr = 10000,
                         sim_fn=function(x) similarityMH(x,k=2,n_hash=50),
                         cluster_fn=function(x,...) igraph::cluster_louvain(x,
                                                                            resolution=1.05,...)$membership,
                         cluster_wt=TRUE) {
  if (size_max <= size_min) {
    stop("size_max must be greater than size_min")
  }
  if (length(pep) == 0) {
    stop("empty input sequence vector")
  }
  
  # Create new state environment
  state <- new.env()
  state$out.df <- matrix(nrow = 0, ncol = 2)
  state$itr <- 1
  state$convergence <- 1
  state$filter.df <- NULL
  
  cluster_recursive <- function(pep) {
    
    # Create helper function for logging adapted from Claude AI prompt
    log_message <- function(msg, level="INFO") {
      timestamp <- format(Sys.time(), "%H:%M:%S")
      cat(sprintf("[%s] %s: %s\n", timestamp, level, msg))
    }
    
    if (state$itr > max_itr) {
      log_message("Maximum function calls reached", "WARNING")
      state$convergence<-0 # change status to not converged since max itr reached
      return(state$out.df)
    }
    
    pep.sim <- sim_fn(pep)  #custom function for similarity matrix generation
    
    threshold <- quantile(pep.sim[upper.tri(pep.sim)],thresh_p) #quantile based threshold
    
    pep.sim[pep.sim<threshold] <- 0 # remove edges from nodes with similarity below threshold
    c.index <- netcluster(pep.sim,cluster_func = cluster_fn,cluster_weight=cluster_wt) #cluster id
    pep.ref <- cbind(pep, c.index) # combine cluster id with sequences
    c.size <- tabulate(c.index) # count each cluster size
    id.itr <- which(c.size > size_max) # cluster id above max size
    id.rm <- which(c.size < size_min) # cluster id below min size
    
    state$filter.df <- c(state$filter.df,pep.ref[pep.ref[,2] %in% id.rm,1]) #store filtered sequences
    
    # stop or recursion conditions for output
    if (length(id.itr) == 0) {  
      out.pep <- pep.ref[!pep.ref[,2] %in% id.rm,] # filter out those under minimum length
      if (nrow(out.pep) > 0) { #ensure non-empty output
        out.pep[,2] <- paste0(state$itr, ".", out.pep[,2]) #combine iteration and cluster to ensure uniqueness
        state$out.df <- rbind(state$out.df, out.pep) # combine output df
      }
    } else {
      pep.out <- pep.ref[(!pep.ref[,2] %in% id.rm) & (!pep.ref[,2] %in% id.itr),] # recursion condition
      if (nrow(pep.out) > 0) {
        pep.out[,2] <- paste0(state$itr, ".", pep.out[,2])#combine iteration and cluster to ensure uniqueness
        state$out.df <- rbind(state$out.df, pep.out)# combine output df
        
      }
      
      # filter remaining clusters
      pep.new <- pep.ref[pep.ref[,2] %in% id.itr,]#clusters with size > max
      loop.lvl <- unique(pep.new[,2])#set levels for loop
      
      # loop for remaining qualified clusters
      for (i in seq_along(loop.lvl)) {
        sub.df <- pep.new[pep.new[,2] == loop.lvl[i], 1] #filter ith cluster
        state$itr <- state$itr + 1 #increment for iteration/function call
        cluster_recursive(sub.df) #recursive call
      }
    }
    
    return(list(clustered_seq=state$out.df,
                filtered_seq=state$filter.df))
  }
  
  # run recursive clustering
  result <- cluster_recursive(pep)
  
  # Final status report adapted from claude AI output
  if (state$convergence==1){
    cat(sprintf("\nClustering complete:\n"))
  }else{
    cat(sprintf("\nClustering incomplete, consider adjusting parameters:\n"))
  }
  cat(sprintf("Total function calls (clusters broken): %d\n", state$itr))
  
  rm(state) # reset state global 
  
  return(result)
}


#' Generate consensus sequence
#'
##' @param df A matrix or data frame where the first column contains sequences and the second column contains their corresponding cluster assignments.
#' 
#' @return A matrix with two columns:
#' \describe{
#'   \item{Cluster ID}{Unique identifier for each cluster.}
#'   \item{Consensus Sequence}{The consensus amino acid sequence for the cluster.}
#' }
#' @export
#' 
#' @importFrom Biostrings AAStringSet
#' @importFrom DECIPHER AlignSeqs ConsensusSequence
#' 
#' @examples
#' library(DynaAlign)
#' 
#' # Create a mock clustered sequence matrix with at least two sequences per cluster
#' clustered_seq <- matrix(c(
#'   "AAAA", "1",
#'   "AAAB", "1",
#'   "AAAC", "1",
#'   "BBBB", "2",
#'   "BBBC", "2",
#'   "BBBB", "2",
#'   "CCCC", "3",
#'   "CCCD", "3"
#' ), ncol = 2, byrow = TRUE)
#' 
#' # Generate consensus sequences
#' consensus <- clusterconsensus(clustered_seq)
clusterconsensus <- function(df) {
  cluster.id <- unique(df[,2])
  out.df <- matrix(nrow=0,ncol=2)
  for (i in 1:length(cluster.id)) {
    df.sub <- df[which(df[,2] == cluster.id[i]),1]
    aa.set <- Biostrings::AAStringSet(df.sub)
    aa.set.align <- DECIPHER::AlignSeqs(aa.set)
    con.seq <- as.character(DECIPHER::ConsensusSequence(aa.set.align))
    out.df <- rbind(out.df, c(cluster.id[i], con.seq))
  }
  return(out.df)
}

#' Plot consensus sequences for each cluster in a clustered network
#'
#' @param df Input clusterconsensus function output.
#' @param k_size Minhash kmer size.
#' @param hash_size Number of hash functions.
#' @param threshold_p Binary threshold for adjacency matrix.
#' @param sens Louvain algorithm sensitivity.
#' @param ... Additional arguments passed to igraph plot function.
#'
#' @return A graph visualization of the consensus sequence network.
#' @export
#'
#' @importFrom igraph graph_from_adjacency_matrix E cluster_louvain V layout_with_fr
#' 
#' @examples
#' # Load necessary libraries
#' library(DynaAlign)
#' library(igraph)
#'
#' # Create a mock clustered sequence matrix
#' clustered_seq <- matrix(c(
#'   "AAAA", "1",
#'   "AAAB", "1",
#'   "BBBB", "2",
#'   "BBBC", "2"
#' ), ncol = 2, byrow = TRUE)
#'
#' # Define a mock clusterconsensus function (for example purposes only)
#' clusterconsensus <- function(df) {
#'   unique_clusters <- unique(df[,2])
#'   consensus <- sapply(unique_clusters, function(cluster) {
#'     sequences <- df[df[,2] == cluster, 1]
#'     if(length(sequences) < 2){
#'       return(sequences)
#'     } else {
#'       # Simple consensus: return the first sequence as a placeholder
#'       return(sequences[1])
#'     }
#'   })
#'   return(data.frame(`Cluster ID` = unique_clusters, 
#'       `Consensus Sequence` = consensus, stringsAsFactors = FALSE))
#' }
#'
#' # Define a mock minhash function (for example purposes only)
#' minhash <- function(sequences, k_size, hash_size) {
#'   # Create a random similarity matrix with higher similarity for identical sequences
#'   n <- length(sequences)
#'   mat <- matrix(runif(n^2, min = 0, max = 1), nrow = n)
#'   diag(mat) <- 1  # Similarity of a sequence with itself
#'   return(list(dist_matrix = mat))
#' }
#'
#' # Generate consensus sequences
#' consensus_seq <- clusterconsensus(clustered_seq)
#'
#' # Plot consensus sequences network
#' consensusplot(consensus_seq)
consensusplot<-function(df,
                        k_size = 2, 
                        hash_size = 50,
                        threshold_p = 0.8,
                        sens = 1.05,
                        ...) {
  #similarity matrix and adjacency matrix
  df.hash <- minhash(df[,2], k_size, hash_size)
  df.hash <- df.hash$dist_matrix
  threshold <- quantile(df.hash[upper.tri(df.hash)],threshold_p)
  df.hash[df.hash<threshold] <- 0
  #plot call
  g <- igraph::graph_from_adjacency_matrix(df.hash, mode="upper", weighted=TRUE) # base plot
  g.weight <- igraph::E(g)$weight # edge weight
  g.cluster <- igraph::cluster_louvain(g,weights = g.weight, resolution = sens) # cluster using weights
  
  g.layout <- igraph::layout_with_fr(g, weights = g.weight) #set layout for plot
  igraph::V(g)$name <- df[,1]#set node names to cluster name
  g.out <- plot(g.cluster, g, layout = g.layout,...)
  return(g.out)
}