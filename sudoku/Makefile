JC = javac

CLASSES = \
	gui/ConfigurationPanel.class \
	gui/ConfigurationWindow.class \
	gui/FamilyWindow.class \
	gui/MainWindow.class \
	gui/ProgressPanel.class \
	gui/SimpleWindow.class \
	gui/SolutionDetailsDisplay.class \
	gui/SolutionDisplay.class \
	gui/SolutionsPanel.class \
	gui/TextDisplay.class \
	model/Configuration.class \
	model/Family.class \
	model/Generation.class \
	model/Population.class \
	model/Solution.class \
	operators/CyclicOperator.class \
	operators/GeneticOperations.class \
	operators/GeneticOperator.class \
	operators/UniformOperator.class \
	test/Main.class \
	test/SolutionReader.class

all: $(CLASSES)

%.class: %.java
	$(JC) $<

classes: $(CLASSES:.java=.class)

.PHONY: clean
clean:
	find -type f -name '*.class' -delete
