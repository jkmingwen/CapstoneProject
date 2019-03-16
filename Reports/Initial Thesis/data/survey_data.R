## Invoke appropriate libraries
library(tidyr)  # for easy restructuring of data structures
library(RColorBrewer)  # for colours
library(plotly)  # for radar graphs

## Set working directory
setwd("~/Documents/CapstoneProject/Reports/Initial Thesis/data/")
## Read in data
quant_data <- read.csv("survey_quantitative.csv", stringsAsFactors = FALSE);
qual_data <- read.csv("survey_qualitative.csv", stringsAsFactors = FALSE);

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

# summary(sapply(quant_engagement[,2:13], as.numeric)[1,])
# summary(sapply(quant_engagement[,2:13], as.numeric)[2,])
# summary(sapply(quant_engagement[,2:13], as.numeric)[3,])

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
# data ready for visualisations

# Visualise qualitative data with radar graphs
# "Engagement" radar graph
plot_ly(type = 'scatterpolar',
        fill = 'toself') %>% 
  add_trace(r = qual_engagement[[1]],
            theta = rownames(qual_engagement),
            color = graph_cols[1],
            name = 'Circles') %>%
  add_trace(r = qual_engagement[[2]],
            theta = rownames(qual_engagement),
            color = graph_cols[2],
            name = 'Clouds') %>%
  add_trace(r = qual_engagement[[3]],
            theta = rownames(qual_engagement),
            color = graph_cols[3],
            name = 'Shimmer') %>%
  layout(polar = 
           list(radialaxis = 
                  list(visible = T, 
                       range = c(0, as.numeric(max(unlist(qual_engagement[1:3])))))),
         title = "Engagement")

# "Intuitiveness" radar graph
plot_ly(type = 'scatterpolar',
        fill = 'toself') %>% 
  add_trace(r = qual_intuitivity[[1]],
            theta = rownames(qual_intuitivity),
            color = graph_cols[1],
            name = 'Circles') %>%
  add_trace(r = qual_intuitivity[[2]],
            theta = rownames(qual_intuitivity),
            color = graph_cols[2],
            name = 'Clouds') %>%
  add_trace(r = qual_intuitivity[[3]],
            theta = rownames(qual_intuitivity),
            color = graph_cols[3],
            name = 'Shimmer') %>%
  layout(polar = 
           list(radialaxis = 
                  list(visible = T, 
                       range = c(0, as.numeric(max(unlist(qual_intuitivity[1:3])))))),
         title = "Intuitiveness")

# "improvement" radar graph
plot_ly(type = 'scatterpolar',
        fill = 'toself') %>% 
  add_trace(r = qual_improvement[[1]],
            theta = rownames(qual_improvement),
            color = graph_cols[1],
            name = 'Circles') %>%
  add_trace(r = qual_improvement[[2]],
            theta = rownames(qual_improvement),
            color = graph_cols[2],
            name = 'Clouds') %>%
  add_trace(r = qual_improvement[[3]],
            theta = rownames(qual_improvement),
            color = graph_cols[3],
            name = 'Shimmer') %>%
  layout(polar = 
           list(radialaxis = 
                  list(visible = T, 
                       range = c(0, as.numeric(max(unlist(qual_improvement[1:3])))))),
         title = "Improvements")
