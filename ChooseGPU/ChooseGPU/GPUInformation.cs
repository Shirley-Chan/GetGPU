using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace ChooseGPU
{
    class MemorySize
    {
        public MemorySize(double ByteSize)
        {
            ByteValue = ByteSize;
            KByteValue = ByteValue / 1024;
            MByteValue = KByteValue / 1024;
            GByteValue = MByteValue / 1024;
        }
        public double ByteValue;
        public double KByteValue;
        public double MByteValue;
        public double GByteValue;
    }
    class GPUInformation
    {
        public GPUInformation(XElement element)
        {
            GPUName = element.Element("name").Value;
            DeviceID = int.Parse(element.Element("hardwareid").Value);
            DedicatedVideoMemory = new MemorySize(double.Parse(element.Element("dedicatedvideomemory").Value));
            DedicatedSystemMemory = new MemorySize(double.Parse(element.Element("dedicatedsystemmemory").Value));
            SharedSystemMemory = new MemorySize(double.Parse(element.Element("sharedsystemmemory").Value));
        }
        public static List<GPUInformation> LoadAllGPUInfo(string GPUInfoFilePath)
        {
            List<GPUInformation> Arr = new List<GPUInformation>();
            XmlRead xml = new XmlRead(GPUInfoFilePath, "gpulist");
            IEnumerable<XElement> Buf = xml.GetElementsInRoot("gpuinfo");
            foreach(XElement i in Buf) Arr.Add(new GPUInformation(i));
            return Arr;
        }
        /// <summary>
        /// GPUの名前
        /// </summary> 
        public string GPUName;
        /// <summary>
        /// GPUのデバイスＩＤ
        /// </summary> 
        public int DeviceID;
        /// <summary>
        /// CPUと共有していない専用ビデオメモリのサイズ
        /// </summary> 
        public MemorySize DedicatedVideoMemory;
        /// <summary>
        /// GPUと共有していない専用システムメモリのサイズ
        /// </summary> 
        public MemorySize DedicatedSystemMemory;
        /// <summary>
        /// 共有システムメモリのサイズ
        /// </summary> 
        public MemorySize SharedSystemMemory;
    }
}
