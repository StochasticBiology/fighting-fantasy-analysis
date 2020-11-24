library(rpart)

swapref <- function(ref) {
if(ref == 1) { return(3) }
if(ref == 3) { return(1) }
return(ref)
}

df = read.table("optimal-strategies-4.txt")
colnames(df) <- c("Kdiff", "Luck", "S_h", "S_o", "v_p", "S")
df$S = sapply(df$S,swapref)
mytree=rpart(S ~ Kdiff + Luck + S_h + S_o, data=df, method="class", control=rpart.control(cp=0.0065))

pdf("optimal-strategies-4-tree.pdf")
plot(mytree, uniform=T)
text(mytree, use.n=T, all=T, cex=.8)
dev.off()


df = read.table("optimal-strategies-3.txt")
colnames(df) <- c("Kdiff", "Luck", "S_h", "S_o", "v_p", "S")
df$S = sapply(df$S,swapref)
mytree=rpart(S ~ Kdiff + Luck + S_h + S_o, data=df, method="class", control=rpart.control(cp=0.01))

pdf("optimal-strategies-3-tree.pdf")
plot(mytree, uniform=T)
text(mytree, use.n=T, all=T, cex=.8)
dev.off()

