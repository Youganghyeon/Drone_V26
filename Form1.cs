using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
// 🌟 GMap 관련 네임스페이스 4개
using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;

namespace GCS
{
    public partial class Form1 : Form
    {
        // 📍 웨이포인트 좌표들을 차곡차곡 담아둘 주머니(리스트)
        private List<PointLatLng> waypointList = new List<PointLatLng>();

        // 📍 지도 위에 마커랑 선을 그릴 투명 도화지(레이어)
        private GMapOverlay waypointOverlay;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // 1. 시작할 때 안쪽 메뉴판(FP_MENU)을 숨겨서 버튼만 바닥에 딱 붙게 함
            FP_MENU.Visible = false;

            // 🗺️ 2. 지도(GMap) 초기화 설정 시작!

            // 안전한 구글 지도 엔진으로 설정 (위성 지도를 원하면 GoogleSatelliteMapProvider로 변경 가능)
            gMapControl1.MapProvider = GoogleMapProvider.Instance;
            GMaps.Instance.Mode = AccessMode.ServerOnly; // 인터넷에서 지도 가져오기

            // 초기 카메라 위치 설정 (예: 서울 시청 근처 좌표)
            gMapControl1.Position = new PointLatLng(37.5665, 126.9780);

            // 지도 줌 레벨 세팅 (숫자가 클수록 확대)
            gMapControl1.MinZoom = 2;
            gMapControl1.MaxZoom = 18;
            gMapControl1.Zoom = 15; // 동네가 딱 잘 보이는 줌 상태

            // 마우스 조작 편의 기능 활성화
            gMapControl1.CanDragMap = true;              // 마우스 드래그로 지도 이동 가능
            gMapControl1.DragButton = MouseButtons.Left; // 마우스 왼쪽 클릭 드래그로 지도 이동

            // 🌟 [에러 나던 부분 수정!] 상단 using 덕분에 그냥 MouseWheelZoomType으로 깔끔하게 매핑 가능
            gMapControl1.MouseWheelZoomType = MouseWheelZoomType.MousePositionWithoutCenter;

            // 🖌️ 3. 웨이포인트를 그릴 도화지(Overlay) 생성 후 지도에 추가
            waypointOverlay = new GMapOverlay("waypoints");
            gMapControl1.Overlays.Add(waypointOverlay);

            // 🖱️ 4. 버전 호환성 없는 안전한 MouseClick 이벤트로 연결!
            gMapControl1.MouseClick += GMapControl1_MouseClick;
        }

        // 🌟 지도를 클릭했을 때 실행되는 에러 없는 핵심 이벤트 함수!
        private void GMapControl1_MouseClick(object sender, MouseEventArgs e)
        {
            // 마우스 왼쪽 버튼을 클릭했을 때만 좌표를 찍음
            if (e.Button == MouseButtons.Left)
            {
                // 🌟 마우스가 클릭된 화면의 X, Y 픽셀 좌표를 실제 위도/경도로 변환해주는 함수
                PointLatLng pointClick = gMapControl1.FromLocalToLatLng(e.X, e.Y);

                double lat = pointClick.Lat; // 위도
                double lng = pointClick.Lng; // 경도

                // 디버그 출력창에 좌표가 잘 찍히나 확인용
                Console.WriteLine($"웨이포인트 추가 -> 위도: {lat}, 경도: {lng}");

                // 1. 좌표 리스트에 저장
                waypointList.Add(pointClick);

                // 2. 클릭한 자리에 파란색 핀 마커 생성
                GMapMarker wpMarker = new GMarkerGoogle(pointClick, GMarkerGoogleType.blue_pushpin);
                wpMarker.ToolTipText = $"{waypointList.Count}번 위치"; // 마우스 올리면 뜸
                waypointOverlay.Markers.Add(wpMarker); // 도화지에 마커 추가

                // 3. 점이 2개 이상 모이면 점들을 파란 선(Route)으로 이어줌
                if (waypointList.Count > 1)
                {
                    // 찌꺼기 안 남게 기존 선은 싹 지우고
                    waypointOverlay.Routes.Clear();

                    // 리스트에 저장된 모든 좌표를 잇는 두께 3짜리 파란색 선 생성
                    GMapRoute pathRoute = new GMapRoute(waypointList, "drone_path");
                    pathRoute.Stroke = new Pen(Color.Blue, 3);

                    waypointOverlay.Routes.Add(pathRoute); // 도화지에 선 추가
                }
            }
        }

        private void B_PID_Click(object sender, EventArgs e)
        {
            P_PID.Visible = !P_PID.Visible;
        }

        private void Menu_Click(object sender, EventArgs e)
        {
            FP_MENU.Visible = !FP_MENU.Visible;
        }

        // 아래 빈 이벤트들은 디자이너 연결용이라 그대로 유지함
        private void P_PID_Paint_2(object sender, PaintEventArgs e) { }
        private void FP_MENU_Paint(object sender, PaintEventArgs e) { }
        private void flowLayoutPanel1_Paint(object sender, PaintEventArgs e) { }
        private void gMapControl1_Load(object sender, EventArgs e) { }
    }
}