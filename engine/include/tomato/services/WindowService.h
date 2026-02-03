#ifndef TOMATO_WINDOWSERVICE_H
#define TOMATO_WINDOWSERVICE_H

#include <memory>

struct GLFWwindow;

namespace tomato
{
    class WindowService;
    class InputService;

    struct WindowData
    {
        WindowService* window;
        InputService* input;

        WindowData(WindowService* w, InputService* i) : window(w), input(i) {}
    };

    /**
     * @brief Service responsible for window creation and OS-level event handling.
     *
     * OS 및 플랫폼(GLFW)과 직접 맞닿는 계층.
     */
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

        void SetWindowUserPointer(InputService* input);

        /**
         * @brief Swaps the front and back buffers.
         *
         * 렌더링이 끝난 후 프레임 단위로 호출해야 한다.
         * VSync 설정 여부에 따라 블로킹이 발생할 수 있다.
         */
        void SwapBuffers();

        /**
         * @brief Polls window and input events from the OS.
         *
         * 키보드/마우스 콜백 등의 이벤트가 처리된다.
         */
        void PollEvents();

        /**
         * @brief Temporary escape-key handling helper.
         * @warning This function is temporary.
         */
        // TODO: UI System 만들면 삭제
        void TMP_CheckEscapeKey();

    private:
        static void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height);

        GLFWwindow* handle_{nullptr};

        std::unique_ptr<WindowData> data_{nullptr};

        int width_;
        int height_;
    };
}

#endif //TOMATO_WINDOWSERVICE_H
