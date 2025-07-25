#include "run_model.h"

RunModel::RunModel() : model{ nullptr }, vocab{ nullptr }, ctx{ nullptr }, 
                       smpl{ nullptr }, formatted{}, prev_len{ 0 }, messages{}, 
                       model_params{}, ctx_params{}, ngl{ 0 }, n_ctx{ 0 }, flag_stop{false} {
}

RunModel::~RunModel() {
    for (auto& msg : messages) {
        free(const_cast<char*>(msg.content));
    }
    llama_sampler_free(smpl);
    llama_free(ctx);
    llama_model_free(model);
}

int RunModel::InitAI(std::string path, int n_gpu_layers, int n_context) {
    if (flag_stop == true) {
        return 99;
    }

    model_path = path; // <- Путь до модели.
    ngl = n_gpu_layers; // <- количество слоев загружаемых в видеокарту.
    n_ctx = n_context; // <- Размер контекстного окна.
    
    if (model_path.empty()) { // <- Проверка пустой ли путь до модели.
        return STATUS_ERROR_PATH_EMPTY;
    }

    if (flag_stop == true) {
        return 99;
    }

    // Вывод ошибок 
    llama_log_set([](enum ggml_log_level level, const char* text, void*) {
        if (level >= GGML_LOG_LEVEL_ERROR) {
            fprintf(stderr, "%s", text);
        }
        }, nullptr);

    if (flag_stop == true) {
        return 99;
    }

    // инициализация модели
    model_params = llama_model_default_params();
    model_params.main_gpu = 1;
    model_params.n_gpu_layers = ngl;

    if (flag_stop == true) {
        return 99;
    }

    model = llama_model_load_from_file(model_path.c_str(), model_params); // <- Загрузка модели.
    if (!model) { // <- Проверка загрузилась ли модель корректно
        return STATUS_ERROR_LOAD_MODEL;
    }

    if (flag_stop == true) {
        return 99;
    }

    // Это словарь модели(llama_vocab)
    // llama_model_get_vocab(model) - функция, для того чтобы достать словарь из файла модели(.gguf)
    vocab = llama_model_get_vocab(model);
    if (!vocab) {
        return STATUS_ERROR_LOAD_VOCAB;
    }

    if (flag_stop == true) {
        return 99;
    }

    // Тут задаются параметры  для контекста(llama_contexr_params)
    // Функция llama_context_default_params() - задает параметры загрузки контекста по умолчанию.
    ctx_params = llama_context_default_params();
    ctx_params.n_ctx = n_ctx;
    ctx_params.n_batch = 512;

    if (flag_stop == true) {
        return 99;
    }

    // А здесь уже происходит инициализация контекста с моделю.
    ctx = llama_init_from_model(model, ctx_params);
    if (!ctx) { // <- Проверка на правильность инициализации контекста.
        return STATUS_ERROR_INIT_CTX;
    }

    if (flag_stop == true) {
        return 99;
    }

    // initialize the sampler
    smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_min_p(0.08f, 1));
    llama_sampler_chain_add(smpl, llama_sampler_init_temp(1.0f));
    llama_sampler_chain_add(smpl, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));

    if (flag_stop == true) {
        return 99;
    }
    
    std::vector<char> formatt(llama_n_ctx(ctx));
    formatted = formatt;

    if (flag_stop == true) {
        return 99;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return STATUS_LOAD_COMPLETE;
}

std::string RunModel::TokenizationMessage(std::string message) {
    const char* tmpl = llama_model_chat_template(model, nullptr);

    // add the user input to the message list and format it
    messages.push_back({ "user", strdup(message.c_str()) });
    int new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
    if (new_len > (int)formatted.size()) {
        formatted.resize(new_len);
        new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
    }
    if (new_len < 0) {
        return "Ошибка! new_len";
    }

    // Удалите предыдущие сообщения, чтобы получить запрос для генерации ответа.
    std::string prompt(formatted.begin() + prev_len, formatted.begin() + new_len);

    return prompt;
}

std::string RunModel::GenerateOutput(std::string prompt,MyFrame* frame) {
    std::string response;

    const bool is_first = llama_kv_self_used_cells(ctx) == 0;

    // tokenize the prompt
    const int n_prompt_tokens = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), nullptr, 0, is_first, true);
    std::vector<llama_token> prompt_tokens(n_prompt_tokens);
    if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), prompt_tokens.size(), is_first, true) < 0) {
        GGML_ABORT("failed to tokenize the prompt\n");
    }

    if (flag_stop == true) {
        return "stop_thread";
    }

    MyEventLoop evt(myEvent_MessageAiView, frame->GetId());

    // prepare a batch for the prompt
    llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());
    llama_token new_token_id;

    while (true) {
        if (flag_stop == true) {
            return "stop_thread";
        }

        // check if we have enough space in the context to evaluate this batch
        int n_ctx = llama_n_ctx(ctx);
        int n_ctx_used = llama_kv_self_used_cells(ctx);
        if (n_ctx_used + batch.n_tokens > n_ctx) {
            printf("\033[0m\n");
            fprintf(stderr, "context size exceeded\n");
            exit(0);
        }

        if (llama_decode(ctx, batch)) {
            GGML_ABORT("failed to decode\n");
        }

        // sample the next token
        new_token_id = llama_sampler_sample(smpl, ctx, -1);

        // is it an end of generation?
        if (llama_vocab_is_eog(vocab, new_token_id)) {
            break;
        }

        // convert the token to a string, print it and add it to the response
        char buf[256];
        int n = llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf), 0, true);
        if (n < 0) {
            GGML_ABORT("failed to convert token to piece\n");
        }
        std::string piece(buf, n);
        printf("%s", piece.c_str());
        fflush(stdout);
        response += piece;
       
        evt.SetOutputAI(piece);

        wxQueueEvent(frame, evt.Clone());

        // prepare the next batch with the sampled token
        batch = llama_batch_get_one(&new_token_id, 1);
        if (flag_stop == true) {
            return "stop_thread";
        }
    }

    evt.SetOutputAI("\n");

    wxQueueEvent(frame, evt.Clone());

    frame->statusLoadModel = STATUS_READY_GET_MESSAGE_USER;

    return response;
}