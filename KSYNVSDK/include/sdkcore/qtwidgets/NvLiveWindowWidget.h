//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 15. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <INvVideoFrame.h>
#include <NvSmartPtr.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>


class CNvStreamingContext;
class CNvLiveWindowContainer;
class CNvLiveWindowShaderProgram;
class CNvLiveWindowHelper;

class CNvLiveWindowWidget : public QOpenGLWidget,
                            protected QOpenGLFunctions
{
    Q_OBJECT

    Q_PROPERTY(FillMode fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged FINAL)
    Q_PROPERTY(PreferredAspectRatio preferredAspectRatio READ preferredAspectRatio WRITE setPreferredAspectRatio NOTIFY preferredAspectRatioChanged FINAL)

    friend class CNvLiveWindowContainer;

public:
    enum FillMode
    {
        FillModePreserveAspectCrop = 0,
        FillModePreserveAspectFit,
        FillModeStretch
    };
    Q_ENUM(FillMode)

    enum PreferredAspectRatio
    {
        PreferredAspectRatioNone = 0,
        PreferredAspectRatio16v9,
        PreferredAspectRatio1v1,
        PreferredAspectRatio9v16
    };
    Q_ENUM(PreferredAspectRatio)

public:
    CNvLiveWindowWidget(QWidget *parent = nullptr);
    ~CNvLiveWindowWidget();

public:
    QPointF MapCanonicalToWidget(const QPointF &ptCanonical);
    QPointF MapWidgetToCanonical(const QPointF &ptWidget);
    QPointF MapNormalizedToWidget(const QPointF &ptNormalized);
    QPointF MapWidgetToNormalized(const QPointF &ptWidget);

    void ClearVideoFrame();

public:
    void setFillMode(FillMode fillMode);
    FillMode fillMode() const { return m_fillMode; }

    void setPreferredAspectRatio(PreferredAspectRatio preferredAspectRatio);
    PreferredAspectRatio preferredAspectRatio() const { return m_preferredAspectRatio; }

signals:
    void fillModeChanged();
    void preferredAspectRatioChanged();

private:
    void SetStreamingContext(CNvStreamingContext *streamingContext);

    void SetVideoFrame(INvVideoFrame *pIVideoFrame,
                       int64_t videoFrameStreamTime);

    void SetDefaultVideoFrameRect(const SNvRectF &defaultVideoFrameRect);

protected:
    virtual int heightForWidth(int w) const;
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    CNvLiveWindowHelper *m_helper;
    CNvStreamingContext *m_streamingContext;

    FillMode m_fillMode;
    PreferredAspectRatio m_preferredAspectRatio;

    TNvSmartPtr<INvVideoFrame> m_pJCurrentVideoFrame;
    int64_t m_currentVideoFrameStreamTime;
    bool m_currentVideoFramePresented;

private:
    static CNvLiveWindowShaderProgram *m_shaderProgram;
    static int m_usageCounter;
};

