library(tidyr)
library(RColorBrewer)
library(plotly)
# Set working directory
setwd("~/Documents/CapstoneProject/Reports/Initial Thesis/data/")
# Read in data
quant_data <- read.csv("survey_quantitative.csv", stringsAsFactors = FALSE);
qual_data <- read.csv("survey_qualitative.csv", stringsAsFactors = FALSE);

# Quantitative data
quant_engagement <- quant_data[1:4]
colnames(quant_engagement) <- as.character(quant_engagement[1,])
quant_engagement <- quant_engagement[2:nrow(quant_engagement),]
quant_engagement <- sapply(quant_engagement, as.numeric)  # Convert to numeric
engagement_mean <- c(mean(quant_engagement[,2]), 
                     mean(quant_engagement[,3]),
                     mean(quant_engagement[,4]))

quant_intuitivity <- quant_data[c(1, 5:7)]
colnames(quant_intuitivity) <- as.character(quant_intuitivity[1,])
quant_intuitivity <- quant_intuitivity[2:nrow(quant_intuitivity),]
quant_intuitivity <- sapply(quant_intuitivity, as.numeric)  # Convert to numeric
intuitivity_mean <- c(mean(quant_intuitivity[,2]), 
                     mean(quant_intuitivity[,3]),
                     mean(quant_intuitivity[,4]))

# Restructure data for grouped bar graph
quant_engagement <- gather(as.data.frame(quant_engagement), 
                           Animation, 
                           Rating, 
                           Circles:Shimmer)
quant_engagement <- spread(as.data.frame(quant_engagement), Participant, Rating)

quant_intuitivity <- gather(as.data.frame(quant_intuitivity), 
                            Animation, 
                            Rating, 
                            Circles:Shimmer)
quant_intuitivity <- spread(as.data.frame(quant_intuitivity), Participant, Rating)

# summary(sapply(quant_engagement[,2:13], as.numeric)[1,])
# summary(sapply(quant_engagement[,2:13], as.numeric)[2,])
# summary(sapply(quant_engagement[,2:13], as.numeric)[3,])

# Visualise data
graph_cols <- brewer.pal(3, "Accent")
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

# Radar graphs
p <- plot_ly(
  type = 'scatterpolar',
  fill = 'toself'
) %>%
  add_trace(
    r = c(39, 28, 8, 7, 28, 39),
    theta = c('A','B','C', 'D', 'E', 'A'),
    name = 'Group A'
  ) %>%
  add_trace(
    r = c(1.5, 10, 39, 31, 15, 1.5),
    theta = c('A','B','C', 'D', 'E', 'A'),
    name = 'Group B'
  ) %>%
  layout(
    polar = list(
      radialaxis = list(
        visible = T,
        range = c(0,50)
      )
    )
  )