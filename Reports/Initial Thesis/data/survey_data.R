## Invoke appropriate libraries
library(tidyr)  # for easy restructuring of data structures
library(RColorBrewer)  # for colours
library(plotly)  # for radar graphs
library(fmsb)  # alternate radar graphs
library(vioplot)  # for violin plots
library(plyr)

add.alpha <- function(col, alpha=1) {  # function to add alpha to rcolorbrewer
  if(missing(col))
    stop("Please provide a vector of colours.")
  apply(sapply(col, col2rgb)/255, 2, 
        function(x) 
          rgb(x[1], x[2], x[3], alpha=alpha))  
}

## Set working directory
setwd("~/Documents/CapstoneProject/Reports/Initial Thesis/data/")
setwd("../Desktop/data")  # for computer lab
## Read in data
quant_data <- read.csv("survey_quantitative.csv", stringsAsFactors = FALSE);
qual_data <- read.csv("survey_qualitative.csv", stringsAsFactors = FALSE);
ranking_data <- read.csv("survey_rankings.csv");

# Quantitative data
# subset quantitative data on "Engagement"
quant_engagement <- quant_data[1:4]
# rename columns and remove first row (originally with name values)
colnames(quant_engagement) <- as.character(quant_engagement[1,])
quant_engagement <- quant_engagement[2:nrow(quant_engagement),]
quant_engagement <- sapply(quant_engagement, as.numeric)  # convert to numeric
# get means for each animation
engagement_mean <- c(mean(quant_engagement[,2]), 
                     mean(quant_engagement[,3]),
                     mean(quant_engagement[,4]))

# subset quantitative data on "Intuitivity"
quant_intuitivity <- quant_data[c(1, 5:7)]
# rename columns and remove first row (originally with name values)
colnames(quant_intuitivity) <- as.character(quant_intuitivity[1,])
quant_intuitivity <- quant_intuitivity[2:nrow(quant_intuitivity),]
quant_intuitivity <- sapply(quant_intuitivity, as.numeric)  # convert to numeric
# get means for each animation
intuitivity_mean <- c(mean(quant_intuitivity[,2]), 
                      mean(quant_intuitivity[,3]),
                      mean(quant_intuitivity[,4]))

# Convert to data frames
quant_engagement <- as.data.frame(quant_engagement)
quant_intuitivity <- as.data.frame(quant_intuitivity)

# Restructure data for grouped bar graph
# combine separate animation columns into one "Animation" column (wide to long)
quant_engagement <- gather(as.data.frame(quant_engagement), 
                           Animation, 
                           Rating, 
                           Circles:Shimmer)
quant_intuitivity <- gather(as.data.frame(quant_intuitivity), 
                            Animation, 
                            Rating, 
                            Circles:Shimmer)
# reshape from long to wide with Animations as rows and participants as cols
quant_engagement <- spread(as.data.frame(quant_engagement), Participant, Rating)
quant_intuitivity <- spread(as.data.frame(quant_intuitivity), Participant, Rating)

# Visualise quantitative data
graph_cols <- brewer.pal(3, "Accent")  # set colour selection
barplot(as.matrix(quant_engagement[,2:13]), beside = TRUE,
        main = "Engagement", xlab = "Participant", ylab = "Rating",
        ylim = c(0, 10), col = graph_cols, legend.text = quant_engagement[,1],
        args.legend = list(x = 50,
                           y = 12,
                           bty = "n"))

barplot(as.matrix(quant_intuitivity[,2:13]), beside = TRUE,
        main = "Intuitivity", xlab = "Participant", ylab = "Rating",
        ylim = c(0, 10), col = graph_cols, legend.text = quant_engagement[,1],
        args.legend = list(x = 50,
                           y = 12,
                           bty = "n"))
## Violin plots for 'Engagement' and 'Intuitiveness' ratings
vioplot(quant_engagement$Circles, 
        quant_engagement$Clouds, 
        quant_engagement$Shimmer,
        names = names(quant_engagement[2:4]),
        col = graph_cols, 
        main = "Violin plot of 'Engagement' ratings", cex.main = 2,
        xlab = "Animation", ylab = "Rating (1-10)", cex.axis = 1.2, cex.lab = 1.2,
        ylim = c(1, 10))

vioplot(quant_intuitivity$Circles, 
        quant_intuitivity$Clouds, 
        quant_intuitivity$Shimmer,
        col = graph_cols, 
        main = "Violin plot of 'Intuitiveness' ratings", cex.main = 2,
        xlab = "Animation", ylab = "Rating (1-10)", cex.axis = 1.2, cex.lab = 1.2,
        ylim = c(1, 10),
        names = names(quant_intuitivity[2:4]))

# Qualitative data
# subsetting raw data
qual_engagement <- qual_data[1:7,]
qual_intuitivity <- qual_data[12:17,]
qual_improvement <- qual_data[20:27,]
# rename columns of subsets
colnames(qual_engagement) <- as.character(qual_engagement[1,])
colnames(qual_intuitivity) <- as.character(qual_intuitivity[1,])
colnames(qual_improvement) <- as.character(qual_improvement[1,])
# remove first rows (originally holding column names)
qual_engagement <- qual_engagement[-1,]
qual_intuitivity <- qual_intuitivity[-1,]
qual_improvement <- qual_improvement[-1,]
# rename rows of subsets
rownames(qual_engagement) <- qual_engagement[[1]]
rownames(qual_intuitivity) <- qual_intuitivity[[1]]
rownames(qual_improvement) <- qual_improvement[[1]]
# remove first columns (originally holding row names)
qual_engagement <- qual_engagement[-1]
qual_intuitivity <- qual_intuitivity[-1]
qual_improvement <- qual_improvement[-1]
# reshape and change to numeric type
qual_engagement <- as.data.frame(t(qual_engagement))
qual_engagement[] <- lapply(qual_engagement, 
                            function(x) as.numeric(as.character(x)))
qual_intuitivity <- as.data.frame(t(qual_intuitivity))
qual_intuitivity[] <- lapply(qual_intuitivity, 
                            function(x) as.numeric(as.character(x)))
qual_improvement <- as.data.frame(t(qual_improvement))
qual_improvement[] <- lapply(qual_improvement, 
                            function(x) as.numeric(as.character(x)))
# Combine maxmin into data frame
qual_engagement <- rbind(rep(max(qual_engagement), ncol(qual_engagement)),
                         rep(0, ncol(qual_engagement)),
                         qual_engagement)
qual_intuitivity <- rbind(rep(max(qual_intuitivity), ncol(qual_intuitivity)),
                         rep(0, ncol(qual_intuitivity)),
                         qual_intuitivity)
qual_improvement <- rbind(rep(max(qual_improvement), ncol(qual_improvement)),
                         rep(0, ncol(qual_improvement)),
                         qual_improvement)
### data ready for visualisations! ###

# Visualise qualitative data with radar graphs
graph_cols_alpha <- add.alpha(graph_cols, 0.5)
## Engagement
radarchart(qual_engagement[1:3,], axistype = 1,  # Circles
           pcol = graph_cols[1], pfcol = graph_cols_alpha[1], plwd = 3,
           seg = max(qual_engagement),
           caxislabels = 0:max(qual_engagement), 
           vlcex = 1.5, axislabcol = "black",
           cglwd = 2, cglcol = "darkgray", cglty = 1,
           title(main = "Circles: Tag counts for 'Engagement'", cex.main = 2))

radarchart(qual_engagement[c(1,2,4),], axistype = 1,  # Clouds
           pcol = graph_cols[2], pfcol = graph_cols_alpha[2], plwd = 3,
           seg = max(qual_engagement),
           caxislabels = 0:max(qual_engagement), 
           vlcex = 1.5, axislabcol = "black",
           cglwd = 2, cglcol = "darkgray", cglty = 1,
           title(main = "Clouds: Tag counts for 'Engagement'", cex.main = 2))

radarchart(qual_engagement[c(1,2,5),], axistype = 1,  # Shimmer
           pcol = graph_cols[3], pfcol = graph_cols_alpha[3], plwd = 3,
           seg = max(qual_engagement),
           caxislabels = 0:max(qual_engagement), 
           vlcex = 1.5, axislabcol = "black",
           cglwd = 2, cglcol = "darkgray", cglty = 1,
           title(main = "Shimmer: Tag counts for 'Engagement'", cex.main = 2))

## Intuitivity
radarchart(qual_intuitivity[1:3,], axistype = 1,  # Circles
           pcol = graph_cols[1], pfcol = graph_cols_alpha[1], plwd = 3,
           seg = max(qual_intuitivity),
           caxislabels = 0:max(qual_intuitivity), 
           vlcex = 1.5, axislabcol = "black",
           cglwd = 2, cglcol = "darkgray", cglty = 1,
           title(main = "Circles: Tag counts for 'Intuitiveness'", cex.main = 2))

radarchart(qual_intuitivity[c(1,2,4),], axistype = 1,  # Clouds
           pcol = graph_cols[2], pfcol = graph_cols_alpha[2], plwd = 3,
           seg = max(qual_intuitivity),
           caxislabels = 0:max(qual_intuitivity), 
           vlcex = 1.5, axislabcol = "black",
           cglwd = 2, cglcol = "darkgray", cglty = 1,
           title(main = "Clouds: Tag counts for 'Intuitiveness'", cex.main = 2))

radarchart(qual_intuitivity[c(1,2,5),], axistype = 1,  # Shimmer
           pcol = graph_cols[3], pfcol = graph_cols_alpha[3], plwd = 3,
           seg = max(qual_intuitivity),
           caxislabels = 0:max(qual_intuitivity), 
           vlcex = 1.5, axislabcol = "black",
           cglwd = 2, cglcol = "darkgray", cglty = 1,
           title(main = "Shimmer: Tag counts for 'Intuitiveness'", cex.main = 2))

## Improvement
radarchart(qual_improvement[1:3,], axistype = 1,  # Circles
           pcol = graph_cols[1], pfcol = graph_cols_alpha[1], plwd = 3,
           seg = max(qual_improvement),
           caxislabels = 0:max(qual_improvement), 
           vlcex = 1.5, axislabcol = "black",
           cglwd = 2, cglcol = "darkgray", cglty = 1,
           title(main = "Circles: Tag counts for 'Improvement'", cex.main = 2))

radarchart(qual_improvement[c(1,2,4),], axistype = 1,  # Clouds
           pcol = graph_cols[2], pfcol = graph_cols_alpha[2], plwd = 3,
           seg = max(qual_improvement),
           caxislabels = 0:max(qual_improvement), 
           vlcex = 1.5, axislabcol = "black",
           cglwd = 2, cglcol = "darkgray", cglty = 1,
           title(main = "Clouds: Tag counts for 'Improvement'", cex.main = 2))

radarchart(qual_improvement[c(1,2,5),], axistype = 1,  # Shimmer
           pcol = graph_cols[3], pfcol = graph_cols_alpha[3], plwd = 3,
           seg = max(qual_improvement),
           caxislabels = 0:max(qual_improvement), 
           vlcex = 1.5, axislabcol = "black",
           cglwd = 2, cglcol = "darkgray", cglty = 1,
           title(main = "Shimmer: Tag counts for 'Improvement'", cex.main = 2))

# Ranking visualisations
rank_col <- brewer.pal(4, "GnBu")
circle_rank <- ranking_data[1:4]
clouds_rank <- ranking_data[5:8]
shimmer_rank <- ranking_data[9:12]

circle_total <- cbind(tabulate(circle_rank$Track.1),
                      tabulate(circle_rank$Track.2),
                      c(tabulate(circle_rank$Track.3), 0),
                      tabulate(circle_rank$Track.4))
colnames(circle_total) <- c("Track 1", "Track 2", "Track 3", "Track 4")
barplot(circle_total, beside = T, main = "Circles Track rankings",
        legend.text = c("#1", "#2", "#3", "#4"),
        ylab = "Frequency", args.legend = list(x = 22.5, y = 7.5), 
        col = rev(rank_col))

clouds_total <- cbind(tabulate(clouds_rank$Track.1),
                      tabulate(clouds_rank$Track.2),
                      c(tabulate(clouds_rank$Track.3), 0),
                      c(tabulate(clouds_rank$Track.4), 0))
colnames(clouds_total) <- c("Track 1", "Track 2", "Track 3", "Track 4")
barplot(clouds_total, beside = T, main = "Clouds Track rankings",
        legend.text = c("#1", "#2", "#3", "#4"),
        ylab = "Frequency", args.legend = list(x = 22.5, y = 10),
        col = rev(rank_col))

shimmer_total <- cbind(tabulate(shimmer_rank$Track.1),
                      tabulate(shimmer_rank$Track.2),
                      c(tabulate(shimmer_rank$Track.3), 0),
                      tabulate(shimmer_rank$Track.4))
colnames(shimmer_total) <- c("Track 1", "Track 2", "Track 3", "Track 4")
barplot(shimmer_total, beside = T, main = "Shimmer Track rankings",
        legend.text = c("#1", "#2", "#3", "#4"),
        ylab = "Frequency", args.legend = list(x = 22.5, y = 8.5),
        col = rev(rank_col))

### Code timings ###
# Read in data
circles_timings <- read.csv("circles_timings.txt", sep = "\n", header = FALSE)
clouds_timings <- read.csv("clouds_timings.txt", sep = "\n", header = FALSE)
shimmer_timings <- read.csv("shimmer_timings.txt", sep = "\n", header = FALSE)

circles_odroid <- read.csv("circles_timings_odroid.txt", sep = "\n", header = FALSE)
clouds_odroid <- read.csv("clouds_timings_odroid.txt", sep = "\n", header = FALSE)
shimmer_odroid <- read.csv("shimmer_timings_odroid.txt", sep = "\n", header = FALSE)

# Plot timings in boxplots
boxboi <- boxplot(c(circles_timings, clouds_timings, shimmer_timings),
                  names = c("Circles", "Clouds", "Shimmer"),
                  main = "Computation timings")
tabulate(boxboi$group)  # number of outliers per animation

boxdroid <- boxplot(c(circles_odroid, clouds_odroid, shimmer_odroid),
                    names = c("Circles", "Clouds", "Shimmer"),
                    main = "Odroid computation timings")
tabulate(boxdroid$group)  # number of outliers per animation
boxdroid2 <- boxplot(c(circles_odroid, clouds_odroid),
                    names = c("Circles", "Clouds"),
                    main = "Odroid computation timings",
                    outline = FALSE)
boxplot(shimmer_odroid, names = "Shimmer")

