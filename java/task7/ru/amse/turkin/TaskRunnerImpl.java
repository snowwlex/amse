package ru.amse.turkin;


public class TaskRunnerImpl<X,Y> implements TaskRunner {
    private int threads_number;

    public TaskRunnerImpl(int threads_number) {
        this.threads_number = threads_number;
        //if (threads_number < 1) {   throw Exception; }
    }

    public <X, Y> X run(Task<X, Y> task, Y value) {
        return null;
    }
}
