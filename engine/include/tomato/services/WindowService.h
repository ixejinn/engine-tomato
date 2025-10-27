#ifndef TOMATO_WINDOWSERVICE_H
#define TOMATO_WINDOWSERVICE_H

struct GLFWwindow;

namespace tomato
{
    class WindowService
    {
    public:
        WindowService(int width, int height, const char* title);
        ~WindowService();

        GLFWwindow* GetHandle() { return handle_; }

        int GetWidth() const { return width_; }
        int GetHeight() const { return height_; }

        [[nodiscard]] bool ShouldClose() const;
        void RequestClose();

        // 화면 갱신
        void SwapBuffers();

        // 입력/창 이벤트 갱신
        void PollEvents();

        // !!! TEMPORAL FUNCTION !!!
        void TMP_CheckEscapeKey();

    private:
        static void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height);

        GLFWwindow* handle_{nullptr};

        int width_;
        int height_;
    };
}

#endif //TOMATO_WINDOWSERVICE_H
