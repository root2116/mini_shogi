#include "trainer.h"
#include "optimizer.h"
#include "adam.h"
#include "sgd.h"
#include "value_net.h"

#define LEAK_DETECT
#ifdef LEAK_DETECT
#include "leakdetect.h"
#define init leak_detect_init
#define malloc(s) leak_detelc_malloc(s, __FILE__, __LINE__)
#define free leak_detect_free
#define check leak_detect_check
#else
#define init()
#define check()
#endif


#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>



static void train_step(Trainer trainer, int iter_num)
{
 

    int *batch_index = choice(trainer->train_size, trainer->mini_batch_size);

    Tensor x_batch = create_data_batch_tensor(trainer->train_data, batch_index, trainer->mini_batch_size);

    
    Vector t_batch = create_label_batch_vector(trainer->train_labels, batch_index, trainer->mini_batch_size);
   
    
    Params grads = trainer->net->gradient(trainer->net, x_batch, t_batch);
    

    trainer->optimizer->update(trainer->optimizer, trainer->net->params, grads);

  
    const double loss = trainer->net->loss(trainer->net, x_batch, t_batch);

   
    printf("train loss:%lf\n", loss);
    add_w(trainer->train_loss_list, loss);
  

    if (trainer->current_iter % trainer->iter_per_epoch == 0)
    {
        const double train_acc = trainer->net->accuracy(trainer->net, trainer->train_data, trainer->train_labels);
        const double test_acc = trainer->net->accuracy(trainer->net, trainer->test_data, trainer->test_labels);

        if (trainer->verbose)
        {
            printf("epoch:%d train acc, test acc | %lf, %lf\n", trainer->current_epoch, train_acc, test_acc);
        }
        
        
        add_w(trainer->train_acc_list, train_acc);
        add_w(trainer->test_acc_list, test_acc);
        

        ++(trainer->current_epoch);
    }

    ++(trainer->current_iter);

    free(batch_index);
    
    free_tensor(x_batch);
  

    free_vector(t_batch);
 
    free(grads);
 
}

static void train(Trainer this){
    if(this->optimizer_type == Adam){
        this->optimizer = new_adam(this->learning_rate, 0.9,0.999);
    }else if(this->optimizer_type == SGD){
        this->optimizer = new_sgd(this->learning_rate);
    }

    for (int i = 0; i < this->max_iter; ++i){
        
        train_step(this,i);
    }
}

Trainer new_trainer(ValueNet net, 
                    Tensor train_data,
                    Vector train_labels,
                    Tensor test_data,
                    Vector test_labels,
                    int epochs,
                    int optimizer_type,
                    int mini_batch_size,
                    int train_size,
                    int test_size,
                    double learning_rate,
                    bool verbose)
{
    Trainer instance = malloc(sizeof(*instance));

    instance->net = net;
    instance->train_data = train_data;
    instance->train_labels = train_labels;
    instance->test_data = test_data;
    instance->test_labels = test_labels;
    instance->epochs = epochs;
    instance->optimizer_type = optimizer_type;
    instance->mini_batch_size = mini_batch_size;
    instance->train_size = train_size;
    instance->test_size = test_size;
    instance->learning_rate = learning_rate;
    instance->verbose = verbose;

    instance->iter_per_epoch = train_size / mini_batch_size;
    instance->max_iter = instance->epochs * instance->iter_per_epoch;

    instance->train_loss_list = new_list_w();
    instance->train_acc_list = new_list_w();
    instance->test_acc_list = new_list_w();

    instance->current_iter = 0;
    instance->current_epoch = 0;

    instance->train = train;

    return instance;

}